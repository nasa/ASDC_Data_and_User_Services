
/*this is sample c code for reading the lite level 1 data*/
/*this code assumes that a short is 2 bytes and a float and a long are 4*/
/*compile as: cc -o readlite L1read.c  */
/*run as: readlite filename */

/* Two update Notes Added on October 2016 */
/* Note #1 */
/* The Altitude Profile in not included in the LITE Level 1 data product and it must be calculated.*/

/*The altitude profile contains 3000 elements. The altitudes are separated by 15 m.*/

/*The first altitude is 40 km. The remaining altitudes are calculated by subtracting 0.015 km per element.*/

/*For example, the first 4 altitude values are: 40.0, 39.9850, 39.9700, 39.9950.*/
/*The last 4 altitude values are: -4.9400, -4.9550, -4.9700, -4.9850.*/
/*The altitude values near a "zero" altitude are: 0.0250, 0.0100, -0.0050, -0.0200.*/
/*A double precision floating point format was used to calculated the altitude profile values.*/

/* Note #2 */
/* This code contains a check to determine if the data are corrupted.*/
/* In the event that the code detects this error, it is suggested to */
/* byte swap of the LITE data after they are read into the LITE data structure. */


#include <stdio.h>
#include <string.h>

#define MAX_FILENAME 50

/*level 1 structure*/
struct {
  short syncvalue;                /*always 12345*/
  char  majorversionnumber;
  char  minorversionnumber;
  char  datatakeid[7];            /*ascii description*/
  char  orbitnumber;
  long  idnumber;                 /*unique record id*/
  short gmtday;                   /*greenwich mean time*/
  char  gmthour;
  char  gmtmin;
  char  gmtsec;
  char  gmthund;
  short metday;                   /*mission elapsed time*/
  char  methour;
  char  metmin;
  char  metsec;
  char  methund;
  float latitude;                 /*degrees*/
  float longitude;                /*degrees */
  float shuttlealtitude;          /*kilometers*/
  float offnadirangle;            /*degrees*/
  float digitizerondelay;         /*useconds*/
  char  datatakemode;             /*0=day 1=night*/
  char  specialopsmode;           /*0=normal 1=multiscatter 2=gainchange 3=transition*/
  char  profilevalidstatus;       /*0=valid 1=355quest 2=532quest 4=064quest*/
                                  /*8=355invalid 16=532invalid 32=064invalid*/
  char  landwaterflag;            /*0=land 1=water*/
  float surfelevfootprint;        /*kilometers*/
  float metdataalts[18];          /*kilometers*/
  float mettemps[18];             /*kelvin*/
  float alttropopause;            /*kilometers*/
  float temptropopause;           /*kelvin*/
  char  laserselected;            /*0=a 1=b*/
  char  baalignmentstatus;        /*0=not aligned 1=aligned*/
  char  isdbstatus;               /*0=invalid 1=valid*/
  char  badatastatus;             /*0=invalid 1=valid*/
  char  aodatastatus;             /*0=invalid 1=valid*/
  char  motorinmotion;            /*0=stopped 1=motion*/
  char  aperwheelstatus;          /*0=closed 1=large 2=annular 3=small 4=motion*/
  char  backgroundmongain;        /*0=day 1=night*/
  char  surfacemode355;           /*0=no 1=yes*/
  char  dbattenuation355;         /*db*/
  short numbersatabovesurf355;    /*num samples*/
  float highestsatsample355;      /*kilometers*/
  short numberunderflows355;      /*num samples*/
  char  filterstatus355;          /*0=out 1=in 2=motion*/
  char  calibrationstatus355;     /*0=estimated 1=calculated*/
  float calibrationfactor355;
  char  baselinerippleremvd355;   /*0=no 1=yes*/
  char  oscillationremoved355;    /*0=no 1=yes*/
  char  backgroundvalue355;       /*raw value from instrument*/
  char  highvoltage355enabled;    /*0=disabled 1=enabled*/
  float highvoltage355;           /*volts (355 nm measured high voltage)*/
  float energymonitor355;         /*mjoules*/
  float pmtgain355;
  char  baselinesubmethod355;     /*0=slope 1 = averaged*/
  char  subregionunderflow355;    /*0=none 1=underflow in baseline subtraction region*/
  char  anomalousprof355;         /*0=normal 1=anomalous*/
  char  fillbyte1;
  char  surfacemode532;           /*0=atmospheric 1=transition 2=surface*/
  char  dbattenuation532;         /*db*/
  short numbersatabovesurf532;    /*num samples*/
  float highestsatsample532;      /*kilometers*/
  short numberunderflows532;      /*num samples*/
  char  filterstatus532;          /*0=out 1=in 2=motion*/
  char  calibrationstatus532;     /*0=estimated 1=calculated*/
  float calibrationfactor532;
  char  baselinerippleremvd532;   /*0=no 1=yes*/
  char  oscillationremoved532;    /*0=no 1=yes*/
  char  backgroundvalue532;       /*raw value from instrument*/
  char  highvoltage532enabled;    /*0=disabled 1=enabled*/
  float highvoltage532;           /*volts (532 nm measured high voltage)*/
  float energymonitor532;         /*mjoules*/
  float pmtgain532;
  char  baselinesubmethod532;     /*0=slope 1 = averaged*/
  char  subregionunderflow532;    /*0=none 1=underflow in baseline subtraction region*/
  char  anomalousprof532;         /*0=normal 1=anomalous*/
  char  fillbyte2;
  char  surfacemode064;           /*0=atmospheric 1=transition 2=surface*/
  char  dbattenuation064;         /*db*/
  short numbersatabovesurf064;    /*num samples*/
  float highestsatsample064;      /*kilometers*/
  short numberunderflows064;      /*num samples*/
  char  filterstatus064;          /*0=out 1=in 2=motion*/
  char  calibrationstatus064;     /*0=estimated 1=calculated*/
  float calibrationfactor064;
  char  baselinerippleremvd064;   /*0=no 1=yes*/
  char  oscillationremoved064;    /*0=no 1=yes*/
  char  backgroundvalue064;       /*raw value from instrument*/
  char  highvoltage064enabled;    /*0=disabled 1=enabled*/
  float highvoltage064;           /*volts*/
  float energymonitor064;         /*mjoules*/
  float pmtgain064;
  char  baselinesubmethod064;     /*0=slope 1 = averaged*/
  char  subregionunderflow064;    /*0=none 1=underflow in baseline subtraction region*/
  char  anomalousprof064;         /*0=normal 1=anomalous*/
  char  fillbyte3;
  char  timeedsinthour;           /*unadjusted eds time*/
  char  timeedsintmin;
  char  timeedsintsec;
  char  timeedsinthund;
  char  level0fileidnumber;       /*level 0 directory number*/
  char  level0fileidletter;       /*level 0 file letter*/
  char  reserved[6];
  float highvoltage355cmd;        /*volts (355 nm commanded high voltage)*/
  float highvoltage532cmd;        /*volts (532 nm commanded high voltage)*/
  char  reserved2[4];
  float b0_355;                   /*baseline subtraction value*/
  float b0_532;                   /*baseline subtraction value*/
  float b0_064;                   /*baseline subtraction value*/
  char  outofrng355abv40;         /*0=none,1=underflow,2=overflow,3=underflow and overflow*/
  char  outofrng532abv40;         /*0=none,1=underflow,2=overflow,3=underflow and overflow*/
  char  outofrng064abv40;         /*0=none,1=underflow,2=overflow,3=underflow and overflow*/
  char  outofrange355[375];       /*packed bit array for underflow or overflow*/
  char  outofrange532[375];       /*packed bit array for underflow or overflow*/
  char  outofrange064[375];       /*packed bit array for underflow or overflow*/
  short top355;                   /*index of highest valid count*/
  short bottom355;                /*index of lowest valid count*/
  short top532;                   /*index of highest valid count*/
  short bottom532;                /*index of lowest valid count*/
  short top064;                   /*index of highest valid count*/
  short bottom064;                /*index of lowest valid count*/
  float profile355[3000];         /*counts*/
  float profile532[3000];         /*counts*/
  float profile064[3000];         /*counts*/
} astruct;

int main(int argc,char *argv[]){

FILE *fp;
char filename[MAX_FILENAME];
short i;

if(argc < 2) {
  printf("Usage: readlite filename\n");
  printf("you must supply a file name as an argument\n");
  return 0;
}

strcpy(filename,argv[1]);

if((fp = fopen(filename,"rb")) == NULL){
  printf("can't open input file\n");
  exit(0);
}

while(fread(&astruct,sizeof(astruct),1,fp)){

  if(astruct.syncvalue != 12345){
    printf("bad syncvalue\n");
    exit(0);
  }

  printf("GMT Day: %i Time: %2.2i:%2.2i:%2.2i.%2.2i\n\n",astruct.gmtday,astruct.gmthour,
    astruct.gmtmin,astruct.gmtsec,astruct.gmthund);

  printf("       355        532       1064\n");
  printf(" ---------  ---------  ---------\n");

  for(i=0;i<3000;i++) {
    printf("%10.2f %10.2f %10.2f\n",
    astruct.profile355[i],astruct.profile532[i],astruct.profile064[i]);
  }
  printf("\n");
}

fclose(fp);

}