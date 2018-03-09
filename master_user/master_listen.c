//Runs on master gumstix
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>
//rfcomm -r connect 0 00:03:19:50:29:45 1 &
//passkey-agent 0000 40:EF:4C:B0:53:39 &
int main(int argc, char **argv) {
  int pFile,proc, proc2;
  // Opens to device file
  char slave_msg[1];
  char master_msg[1];
  char master_msg2[1];
  int last_played =0 ;
  /*system("madplay /media/card/mp3/ForeverYoung.mp3 -r 44100 --output=wave:- | aplay -D jbl &");
//madplay /home/root/mp3/Iamthedanger.mp3 -r 44100 --output=wave:- | aplay -D jbl 

  sleep(10);
  system("killall madplay");

  system("madplay /media/card/mp3/GladYouCame.mp3 -r 44100 -A12 --output=wave:- | aplay -D bose &");
  sleep(100);
  */
  while (1){
    //printf("reading\n");
    pFile = open("/dev/rfcomm0", O_RDONLY);
    
    //Reading Bluetooth slave
    if (pFile < 0) {
      fprintf (stderr, "bluetooth slave master communication not yet established\n");
      return 1;
    }
    proc = open("/proc/sensor", O_RDONLY);
    if (proc < 0) {
      fprintf (stderr, "sensor communication not yet established\n");
      return 1;
    }
    proc2 = open("/proc/sensor2", O_RDONLY);
    if (proc2 < 0) {
      fprintf (stderr, "sensor2 communication not yet established\n");
      return 1;
    }
    
    //if( (read(pFile, slave_msg, 1) > 0) && (read(proc, master_msg,1) > 0) && (read(proc2, master_msg2,1) > 0)){
    read(pFile, slave_msg, 1);
    read(proc, master_msg,1);
    read(proc2, master_msg2,1);
      // 	read(proc, master_msg, 1);
      if (master_msg[0]=='2' && master_msg2[0]=='2' && slave_msg[0]=='2'){  //play i am danger
	if (last_played!=1){
	  last_played=1;
	  printf("i am danger\n");
	  system("madplay /home/root/mp3/Iamthedanger.mp3 -r 44100 --output=wave:- | aplay -D jbl&");
	  usleep(1500000);
	  system("killall madplay");
	  
	}
      }
      else if (master_msg[0]=='2'){  //play danger ahead
	if (last_played!=2){
	  last_played=2;
	  printf("danger ahead\n");
	  system("madplay /home/root/mp3/DangerAhead.mp3 -r 44100 --output=wave:- | aplay -D jbl&");
	  usleep(1500000);
	  system("killall madplay");
	  
	}
      }
      else if (master_msg2[0]=='2'){  //play danger behind
	if (last_played!=3){
	  last_played=3;
	  printf("danger behind\n");
	  system("madplay /home/root/mp3/DangerBehind.mp3 -r 44100 --output=wave:- | aplay -D jbl&");
	  usleep(1500000);
	  system("killall madplay");
	  
	}
      }
      else if (slave_msg[0]=='2'){ //danger step
	if (last_played!=4){
	  last_played=4;
	  printf("danger step\n");
	  system("madplay /home/root/mp3/DangerStep.mp3 -r 44100 --output=wave:- | aplay -D jbl&");
	  usleep(1500000);
	  system("killall madplay");
	  
	}
      }
      else if (master_msg[0]=='1'){ //warning ahead
	if(last_played!=5){
	  last_played=5;
	  printf("warning ahead\n");
	  system("madplay /home/root/mp3/Warningahead.mp3 -r 44100 --output=wave:- | aplay -D jbl&");
	  usleep(1500000);
	  system("killall madplay");

	}		      
      }
	else if (master_msg2[0]=='1'){ //warning behind
	if(last_played!=6){
	  last_played=6;
	  printf("warning behind\n");
	  system("madplay /home/root/mp3/Warningbehind.mp3 -r 44100 --output=wave:- | aplay -D jbl&");
	  usleep(1500000);
	  system("killall madplay");

	}		      
      }
      else if (slave_msg[0]=='1'){ //warning step
	if(last_played!=7){
	  last_played=7;
	  printf("warning step\n");
	  system("madplay /home/root/mp3/Warningstep.mp3 -r 44100 --output=wave:- | aplay -D jbl&");
	  usleep(1500000);
	  system("killall madplay");

	}		      
      }
      else if (slave_msg[0]=='0' && master_msg[0]=='0' &&  master_msg2[0]=='0' ){ //Safe
	if(last_played!=8){
	  last_played=8;
	  printf("safe\n");
	  system("madplay /home/root/mp3/Safe.mp3 -r 44100 --output=wave:- | aplay -D jbl&");
	  usleep(1500000);
	  system("killall madplay");

	}		      
      }
      
      //play obstacle in foot message
      
      //	  		close(pFile);
      //}
    	  		close(pFile);
			close(proc);
			close(proc2);
    
  }
  
  return 0;
  
}
