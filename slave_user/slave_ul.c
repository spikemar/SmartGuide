#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

//rfcomm -r connect 0 00:03:19:50:29:45 1 &
//passkey-agent 0000 40:EF:4C:B0:53:39 &
int main(int argc, char **argv) {
  int pFile, rfcomm;
  // Opens to device file
  char word[1];

	while (1){
	  pFile = open("/proc/sensor", O_RDONLY);
	  rfcomm = open("/dev/rfcomm0", O_WRONLY);

	  if (pFile < 0) {
	    fprintf (stderr, "proc file not found\n");
	    return 1;
	  }
	  if (rfcomm < 0) {
	    fprintf (stderr, "rfcomm file not found\n");
	    return 1;
	  }
	  read(pFile, word, 1);
	  
	  write(rfcomm,word, 1);
	  //printf("%s\n",word);
	  //if (word[0]=='1')
	  // system("madplay /home/root/audio_files/Safe.mp3 -r 44100 --output=wave:- | aplay -D ameya");
	    //play obstacle in foot message
	    
	 
	  close(pFile);
	  close(rfcomm);
	  sleep(2);
	}

	return 0;
	
}
