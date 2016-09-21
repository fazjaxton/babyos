BabyOS
======

BabyOS is a "game" for toddler-age children who like to touch the computer.
When keys are pressed, images appear and fly around the screen, spinning,
zooming, and moving at random.  I created it for my son, who was excellent at
getting out of the application that I would put him in where I thought he
couldn't cause any damage.

Use
---
BabyOS requires PySFML and PIL.  On Windows, you can download and install these
packages.  On Ubuntu and other Debian systems:

    # apt-get install python-sfml python-pil

To use, copy some pictures into the "pictures" directory.  They can be nearly
any format or size.  The first time the application is run after a new image is
added, it will be discovered and "cached", by converting it to a size and
format useful for the application in the `imgcache` directory.  BabyOS will
associate an image with the key that is the first letter of the filename.  For
instance, if you have `grandma.jpg`, pushing "G" will make that image appear.
Multiple files with the same starting letter will work: an image will be picked
at random.  Then run the application:

    $ python BabyOS.py

Stopping
--------
BabyOS runs full screen to keep children from focusing other applications, and
I wanted to make it nearly impossible for them to accidentally get out of it.
To exit, you must type "stop" as four consecutive characters.


Contributions are wecome if you'd like to add features.  If your child enjoys this, I would love to hear about it!  Please send me an e-mail.
