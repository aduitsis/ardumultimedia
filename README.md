ardumultimedia - USB HID Generic Consumer Device emulation PoC
==============

Newer arduino platforms that do not use a separate FTDI chip can apparently emulate USB devices such as keyboards and mice. To add to that, the examples coming with the arduino software provice a way to do it within a few minutes. However, I couldn't a way to emulate the multimedia keys that many keyboards nowadays provide. For example, my keyboard has volume up and down keys, which are quite useful by the way. I wanted a way for my Arduino Leonardo to be able to emulate even those keystrokes except the "normal" ones. 

As it turns out, to emulate these kinds of keys, a device must implement not only the Generic Desktop profile which allows it to present itself as a keyboard or mouse, but also a Generic Consumer Device profile, which allows it to send these kinds of keystrokes. 

This small piece of code demonstrates how to do this. This is not an exhaustive emulation layer, but just a proof of concept which should hopefully help someone else get up and running faster than I was able to.

Athanasios Douitsis
June 2013
