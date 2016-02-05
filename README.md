# Win32_picture_from_RAM
A simple example how to create RGB24 bitmap in RAM and draw it in a window, without any crap like LoadImage() and GDI+. Surprisingly, after 20 years of Windows programming it is still hard to find useful examples other than LoadImage().

This Win32 example creates an RGB image in RAM, shows it in a window and shows how to update an image dynamically. 

Written in Visual Studio 6 plain C as a native Windows app, no classes, just Windows API.
