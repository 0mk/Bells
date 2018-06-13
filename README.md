# Bells

"Bells" is a program to reproduce traditional change-ringing, often performed on bells in church towers, especially in England. For a good introduction to change-ringing, see the article What is Change Ringing on the web. This article and others it links to will explain the campanological buzzwords used in this manual.

The program consists of two parts - the client and the server. The client is the gui you see when "Bells" is started. The server is the process which actually does the ringing. The server is provided as a separate executable. To allow flexibility, the server may be placed in the filesystem independently of the client. Because of this, the client must be configured to tell it the location of the server program. See the help manual for details (Question mark button). 

Once properly configured, the two programs perform seamlessly together as if they were one.  The client and server communicate via tcp/ip.
The pattern of changes to be rung is known as a composition and is supplied as a composition file in place-notation format. Some example composition files are supplied with "Bells".

The server must be started by clicking the green bell button and it will then read the composition file and check it for errors. If all is ok, it will print the name of the first method it found and wait. You must then click the yellow bell button to commence ringing. You will then see the changes appear on the screen as the server rings them. As well as the visual output from the server, it also sends out alsa midi messages, one for each bell.
