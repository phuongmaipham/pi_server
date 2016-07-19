//gcc foo.c -lusb


#include <stdlib.h>
#include <stdio.h>
#include <usb.h>
#include <time.h>

#define CANNON_FIRE 0x10
#define CANNON_TURN_LEFT 0x4
#define CANNON_TURN_RIGHT 0x8
#define CANNON_TURN_UP 0x1
#define CANNON_TURN_DOWN 0x2
#define CANNON_STOP 0x0


usb_dev_handle *launcher;


int
sendMsg (char control)
{
  char msg[8];
  int ii;

  for (ii = 0; ii < 8; ii++)
    msg[ii] = 0x0;
  //send control message
  msg[0] = control;
  int ret = usb_control_msg (launcher, 0x21, 0x9, 0x200, 0, msg, 8, 1000);
  if (ret < 0)
    printf ("Cannot send command, error:\n");
  return ret;
}


void
stop ()
{
  int ret = sendMsg (CANNON_STOP);
  if (ret < 0)
    printf ("Cannot stop, error: \n");
}


void
fire ()
{
  printf ("Fire!\n");
  //need to wait 6.77 seconds to fire
  struct timespec toWait;
  toWait.tv_sec = 6;
  toWait.tv_nsec = 770000000;

  const struct timespec *delay = &toWait;
  struct timespec *remainder;

  //Start launch sequence and wait for launch
  if (sendMsg (CANNON_FIRE) < 0)
    {
      printf ("Cannot fire.\n");
      return;
    }
  nanosleep (delay, remainder);

  //Stop, so no more missiles launch
  stop ();
}


void
turn (char direction, double delay)
{
  if (direction == CANNON_FIRE)
    fire ();
  else if (direction == CANNON_STOP)
    stop ();
  else
    {
      if (delay == 0)
	/* Start moving. This means it keeps moving until you tell it to stop.
	 * Good for feedback loops, bad for not hitting the stops and abusing
	 * the slip clurch.
	 */
	sendMsg (direction);
      else
	{
	  /* Move for the specified time period, then stop.
	   * This is good for making little scripts, but assumes you know
	   * the pose of the gun. It's a shame there's little to no feedback 
	   * from it.
	   */
	  //Convert the delay into a timespec
	  struct timespec toWait;
	  toWait.tv_sec = (int) delay;
	  toWait.tv_nsec = (delay - toWait.tv_sec) * 100000000;

	  const struct timespec *delay = &toWait;
	  struct timespec *remainder;

	  //Start turn and wait for movement
	  sendMsg (direction);
	  nanosleep (delay, remainder);

	  //Stop turning
	  stop ();
	}
    }
}


void
deinit ()
{
  usb_release_interface (launcher, 0);
  usb_close (launcher);
}


int
init ()
{
  struct usb_bus *busses;

  usb_init ();
  usb_find_busses ();
  usb_find_devices ();

  busses = usb_get_busses ();

  struct usb_bus *bus;

  //Iterate through buses and devices, looking for a missile launcher
  for (bus = busses; bus; bus = bus->next)
    {
      struct usb_device *dev;

      for (dev = bus->devices; dev; dev = dev->next)
	{
	  /* Check if this device is a missile launcher  */
	  if (dev->descriptor.idVendor == 0x1941)
	    {
	      launcher = usb_open (dev);

	      //Make sure it's not claimed
	      char driverName[32];
	      int retval = usb_get_driver_np (launcher, 0, driverName, 31);
	      if (retval == 0)
		{
		  printf ("Claimed by %s\n", driverName);
		  retval = usb_detach_kernel_driver_np (launcher, 0);
		  if (retval)
		    {
		      printf ("%s cannot be detached\n", driverName);
		      return 1;
		    }
		  printf ("Detached %s\n", driverName);
		}

	      //Claim it ourselves
	      int claimed = usb_claim_interface (launcher, 0);
	      if (claimed == 0)
		//Rock on, we have a handle on it.
		return 0;
	      else
		{
		  printf ("Could not claim Missile Launcher\n");
		  return 1;
		}
	    }
	}
    }

  printf ("Did not find Missile Launcher. Is it plugged in?\n");
  return 1;
}


int
main (int argc, char **argv)
{

        printf("Content-Length: 1000\n");
        printf("Content-Type: text/plain\n");
        printf("Date: Mon, 13 Jun 2016 15:48:49 GMT\n");
        printf("\n");


  if (init () != 0)
    {
      printf ("Failed to initialize missile launcher\n");
      //Don't call deinit unless init worked
      return 1;
    }

  printf ("Missile launcher... OK!\n");

  //Turn for a time mode
  turn (CANNON_TURN_LEFT, 1.5);
  turn (CANNON_TURN_UP, 1);
  turn (CANNON_TURN_RIGHT, 1.5);
  turn (CANNON_TURN_DOWN, 1);

  //Turn until I say mode
  turn (CANNON_TURN_UP, 0.0);
  sleep (1);
  stop ();
  turn (CANNON_TURN_DOWN, 0);
  sleep (1);
  stop ();

  fire ();

  deinit ();

  return 0;
}
