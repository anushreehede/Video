## **FOSS@BPHC solution**

### **Description**

This program written in C++ allows the user to enter a url of a YouTube playlist and 
1. Save the urls of all the videos in the playlist in a file called 'address.txt'
2. Download all the videos into a specified folder

### **Install the following dependencies/tools (instructions only for Ubuntu)**

1. curl
  > sudo apt-get update  
    sudo apt-get install libcurl4-gnutls-dev  

  **curl** is a cross-platform command line tool to transfer data from or to a server using URL syntax.  
  **libcurl** is a free client-side URL transfer library.
  
2. youtube-dl
  > sudo apt-get install youtube-dl
  
  **youtube-dl** is a small command-line program to download videos from
  YouTube.com and a few more sites. It requires the Python interpreter, version
  2.6, 2.7, or 3.2+, and it is not platform specific. It should work on
  your Unix box, on Windows or on Mac OS X. 
  [(Source)](https://github.com/brandonshin/youtube-dl/blob/master/README.md)
### **Compiling the program**

  > g++ videos.cpp -lcurl

### **Executing the program**

  > ./a.out playlistURL [downloads directory path]

