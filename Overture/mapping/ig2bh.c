/*****************************************************************************/
/*     THIS IS PROPRIETARY SOURCE CODE OF THE NATIONAL GRID PROJECT          */
/*                      All Rights Reserved                                  */
/*     Duplication or use of any form of this file (source, object,          */
/*     or executable) is prohibited without the express written              */
/*     consent of THE NATIONAL GRID PROJECT.                                 */
/*****************************************************************************/

/***************************************************************************/
/*                                                                         */
/*  Module:    ig2bh.c                                                     */
/*  Purpose:   To interpret and transform IGES entities into NGP NURBS     */
/*  Routines:                                                              */
/*      ReadIgesFile   : Main of module                                    */
/*      process_f      : Read IGES file and separate into D and P files    */
/*      entity_process : Read D file and call specific entity to process P */
/*      field_num      : Return number at specified string locations       */
/*      perf_pro       : Process Parameter data                            */
/*      get_matrix     : Read transformation matrix                        */
/*      transform_point                                                    */
/*      transform_curv_cps                                                 */
/*      transform_surface_cps                                              */
/*      create_entry   : Create an entry in the Directory linked list      */
/*      find_entry     : Return the data pointer of a Directory entry      */
/*      ins_100        : Returns Circarc as Nurb Curve                     */
/*      assign_conic_arc                                                   */
/*      ins_104        : Returns Conic as Nurb Curve                       */
/*      ins_106        : Returns Points or Lines                           */
/*      ins_110        : Returns Line                                      */
/*      ins_112        : Returns Cubic Curves as Nurb Curves               */
/*      get_p_cur      : Evaluates points on cubic curve                   */
/*      ins_114        : Returns BiCubic Surfaces as Nurb Surfaces         */
/*      get_p_sur      : Evaluates points on bicubic surface               */
/*      ins_116        : Returns Point                                     */
/*      ins_118        : Returns Ruled Surface as Nurb Surface             */
/*      ins_120        : Returns Surface of Revolution as Nurb Surface     */
/*      ins_122        : Returns Tabulated Cylinder as Nurb Surface        */
/*      ins_126        : Returns Nurb Curve                                */
/*      ins_128        : Returns Nurb Surface                              */
/*      ins_314        : Color Definition                                  */
/*      ins_402        : Reads Associative Instance data                   */
/*      ins_406        : Reads Name of entity                              */
/*      ins_5001       : Returns Discrete Data as Nurb Surface             */
/*                                                                         */
/***************************************************************************/

/*************************************************************/
/*                       INCLUDE FILES                       */
/*************************************************************/
#include <time.h>
#include <ctype.h>
#include <unistd.h>
#include <stdio.h>

#include "data.h"
#include "graphics.h"
#include "interface.h"
#include "events.h"
#include "cursors.h"
#include "files.h"
#include "helputilities.h"
#include "journal.h"
#include "logo.h"
#include "pick.h"
#include "resource.h"
#include "calldata.h"
#include "iges.h"
#include "create_topology.h"
#include "delete_topology.h"
#include "group_topology.h"
#include "NURBS.h"
#include "yh.h"
#include "mathops.h"
#include "misc.h"
#include "NurbAlloc.h"
#include "surf.h"
#include "point.h"
#include "vector.h"
#include "curves.h"
#include "surfaces.h"
#include "gui_app_routines_proto.h"
#include "object_edit.h"
#include "louderror.h"
#include "add_routines.h"
#include "group_edit.h"


#if __cplusplus
extern char *   tempnam(const char *, const char *);
extern FILE *   popen(const char *, const char *);
extern int pclose (FILE *);
#endif

/*************************************************************/
/*                     DEFINE CONSTANTS                      */
/*************************************************************/

static int process;		/*  default process specified */
static int Process;		/*  default process specified */
static int PrintOut;		/*  default process specified */
static Boolean Glue = TRUE;

static char field_d,record_d;
static long savep,entype,form,trans_mtr,Color,parameter,seqnum,
            form_matrix,status,visible,subordinate,
            entity_use,hierarchy,level;
static double red,green,blue;
static char *name = NULL;
static double bound;

static FILE *dfp,*pfp;

static char *Dfile = NULL; /* File name for "D" part of IGES file */
static char *Pfile = NULL; /* File name for "P" part of IGES file */

static int max_seqnum = 0;
static DirEntry_t **DE_array = NULL;

/***************************************************************************/
/*                                                                         */
/*  Function:  ReadIgesFile                                                */
/*  Purpose:   To interpret and transform IGES entities into NGP NURBS     */
/*  Parameters:                                                            */
/*              info is the main data structure containing widget info     */
/*              and the main draw structure.                               */
/*              file_name is the file name of the IGES data                */
/*                                                                         */
/***************************************************************************/

int ReadIgesFile(menuCalldata *info, char *file_name, int compressed)
{
    FILE *fp;
    int   maxdata;
    char string[200];
    double scale,tolerence;
    float f_tmp;
    int units,setnum;
    ApplicationNumber save_current_application;
    char *routine = "ReadIgesFile";

    char *ngp_glue;
    char *ngp_iges_glue;
    char *ngp_iges_process;
    char *ngp_iges_printout;

    time_t st,ft;
    double dt;

    if (info == NULL){
       Error(routine,"info pointer is NULL");
       return 1;
    }

    if (info->fileIO.New == 1)
    {
       if (info->data->Vector_PTR != NULL){
          strcpy(string,"Vector_PTR is not NULL");
          Error(routine,string);
          gui_add_to_message_buffer_(&info,string,strlen(string));
          strcpy(string,"Exit program and restart");
          LoudError(info,string);
          return 1;
       }
       if (info->data->Vertex_PTR != NULL){
          strcpy(string,"Vertex_PTR is not NULL");
          Error(routine,string);
          gui_add_to_message_buffer_(&info,string,strlen(string));
          strcpy(string,"Exit program and restart");
          LoudError(info,string);
          return 1;
       }
       if (info->data->Edge_PTR != NULL){
          strcpy(string,"Edge_PTR is not NULL");
          Error(routine,string);
          gui_add_to_message_buffer_(&info,string,strlen(string));
          strcpy(string,"Exit program and restart");
          LoudError(info,string);
          return 1;
       }
       if (info->data->Face_PTR != NULL){
          strcpy(string,"Face_PTR is not NULL");
          Error(routine,string);
          gui_add_to_message_buffer_(&info,string,strlen(string));
          strcpy(string,"Exit program and restart");
          LoudError(info,string);
          return 1;
       }
       if (info->data->Block_PTR != NULL){
          strcpy(string,"Block_PTR is not NULL");
          Error(routine,string);
          gui_add_to_message_buffer_(&info,string,strlen(string));
          strcpy(string,"Exit program and restart");
          LoudError(info,string);
          return 1;
       }
    }

    if (file_name == NULL)
    {
       Error(routine,"file name is NULL");
       return 1;
    }

    if ((ngp_glue = getenv("NGP_GLUE")) != NULL &&
        strstr(ngp_glue,"FALSE")) Glue = FALSE;

    if ((ngp_iges_glue = getenv("NGP_IGES_GLUE")) != NULL)
    {
      if (strstr(ngp_iges_glue,"FALSE"))
        Glue = FALSE;
      else if (strstr(ngp_iges_glue,"TRUE"))
        Glue = TRUE;
    }

    if ((ngp_iges_process = getenv("NGP_IGES_PROCESS")) != NULL)
    {
      if (strstr(ngp_iges_process,"ALL"))
        Process = 0;
      else if (strstr(ngp_iges_process,"CURVES_ONLY"))
        Process = 1;
      else if (strstr(ngp_iges_process,"SURFACES_ONLY"))
        Process = 2;
      else if (strstr(ngp_iges_process,"NO_GROUPS"))
        Process = 3;
      else
        Process = 0;
    }
    else
      Process = 0;

    if ((ngp_iges_printout = getenv("NGP_IGES_PRINTOUT")) != NULL)
    {
      if (strstr(ngp_iges_printout,"TRUE"))
        PrintOut = 1;
      else
        PrintOut = 0;
    }
    else
      PrintOut = 0;

    if (compressed == 2)
    {
       sprintf(string,"gunzip -c %s",file_name);
       if (( fp=popen(string,"r") ) == NULL )
       {
          sprintf(string," Can't open input file\n%s", file_name);
          Error(routine,string);
          gui_add_to_message_buffer_(&info,string,strlen(string));
          return(-1);
       }
    }
    else if (compressed == 1)
    {
       sprintf(string,"zcat %s",file_name);
       if (( fp=popen(string,"r") ) == NULL )
       {
          sprintf(string," Can't open input file\n%s", file_name);
          Error(routine,string);
          gui_add_to_message_buffer_(&info,string,strlen(string));
          return(-1);
       }
    }
    else
    {
       if (( fp=fopen(file_name,"r") ) == NULL )
       {
          sprintf(string," Can't open input file\n%s", file_name);
          Error(routine,string);
          gui_add_to_message_buffer_(&info,string,strlen(string));
          return(-1);
       }
    }

    /* findout the field dilimeter and record */
    /* delimiter and separate the input file  */
    /* into directory file and parameter file */

    printf("\n>>> READING IGES FILE (%s) <<<\n",file_name);

    st = time(NULL);

    if ((maxdata = process_f(info,fp,&scale,&units,&tolerence)) == 0) 
    {
       if (compressed != 0)
          pclose(fp);
       else
          fclose(fp);

       unlink(Dfile); unlink(Pfile); free(Dfile); free(Pfile);
       Dfile = Pfile = NULL;
       return(-1);
    }

    if (compressed != 0)
       pclose(fp);
    else
       fclose(fp);

    if (PrintOut == 1)
    {
      printf("scale = %g units = %s tolerence = %g bound = %g\n",
             scale,iges_units[units-1],tolerence,bound);
    }

    ft = time(NULL);
    dt = difftime(ft,st);

    printf("\n>>> END PARSING IGES FILE (%g seconds) <<<\n\n",dt);

    save_current_application = info->interface.current_application_number;
    info->interface.current_application_number = GLOBAL;
    setnum = 2;
    gui_set_opt_setting_(&info,&setnum,iges_units[units-1],strlen(iges_units[units-1]));
    setnum = 3;
    f_tmp = scale;
    gui_set_flt_setting_(&info,&setnum,&f_tmp);
    info->interface.current_application_number = save_current_application;

    if (tolerence > 0.) info->data->Tolerence = tolerence;

    entity_process(info,maxdata);

    unlink(Dfile); unlink(Pfile); 
    free(Dfile); free(Pfile); 
    Dfile = Pfile = NULL;

    info->data->Modified = FALSE;

    return (0);
}

/*  this file read in the input file and find out the field delimeter
    record delimiter from the Globe section of the IGES file , and 
    separate the input IGES file to P and D temp files   */

int process_f(menuCalldata *info,FILE *fp,double *scale,int *units,
              double *tolerence)
{
    char buf[RECBUF+2], temp[3], *sp, *Global;
    int  i=0,j,ii;
    int ch, c_shift = 0;
    char string[100];
    int  global = 1;
    char *tempdir = getenv("TMPDIR");
    int dstat = 0, pstat = 0, ddata = 0;
    int Unit = 1;
    double Scale = 1.0;
    double Tolerence = 0.;
    double Bound = 1000000.;
    char syscmd[100];
    int pos;
    char *routine = "process_f";
    FILE *ptr;

    temp[2] = '\0';

    if (info == NULL){
       Error(routine,"Error info is NULL");
       return 0;
    }

    if (fp == NULL){
       Error(routine,"Error file pointer is NULL");
       return 0;
    }

    if (tempdir != NULL)
       sprintf(syscmd,"/bin/rm -f %s/NGP.IGES.*",tempdir);
    else
       sprintf(syscmd,"/bin/rm -f /usr/tmp/NGP.IGES.*",tempdir);

    if ((ptr = popen(syscmd,"r")) != NULL)
       pclose(ptr);

    /*========  Seperate into "D" & "P" files  ========*/
    if (!(Dfile = tempnam(tempdir,"NGP.IGES.")) || !(dfp = fopen(Dfile,"wb+"))){
        Error(routine,"Can't open Dfile for write");
        sprintf(string,"`%s': cannot open Dfile for write");
        gui_add_to_message_buffer_(&info,string,strlen(string));
        return 0;
    }
    if (!(Pfile = tempnam(tempdir,"NGP.IGES.")) || !(pfp = fopen(Pfile,"wb+"))){
        Error(routine,"Can't open Pfile for write");
        sprintf(string,"`%s': cannot open Pfile for write");
        gui_add_to_message_buffer_(&info,string,strlen(string));
        return 0;
    }

    record_d = ';';
    field_d = ',';

    sp = fgets(buf, RECBUF, fp);
    if ((ch = getc(fp)) != '\n')
       ungetc(ch,fp);
    else
    {
       if ((ch = getc(fp)) != '\n')
       {
          ungetc(ch,fp);
          c_shift = 1;
       }
       else
          c_shift = 2;
    }

    while (sp)
    {
        if (c_shift == 1) buf[RECBUF-1] = '\0';
        if (c_shift == 2) buf[RECBUF-1] = '\0';
        if (c_shift == 2) buf[RECBUF] = '\0';
        switch (buf[KEYPOS]) 
        {
        case 'S':
            sp = fgets(buf, RECBUF+c_shift, fp);
            break;
        case 'G':
            Global = (char *)malloc(10*RECBUF*sizeof(char));
            i=0;
            while (buf[KEYPOS] == 'G'){
              for(pos=KEYPOS-1;pos>=0;pos--)
              {
                if (buf[pos] == field_d) break;
                if (buf[pos] == record_d) break;
                if (buf[pos] != ' ')
                {
                   pos = KEYPOS-1;
                   break;
                }
              }
              for(j=0;j<=pos;j++,i++)
                Global[i] = buf[j]; 
              sp = fgets(buf, RECBUF+c_shift, fp);
            }
            Global[i] = '\0';
            pos = i;
            if ( global == 1 ) 
            {
               if (Global[0] == ',' )
               {
                  field_d = ',' ; global++;
                  if ( Global[1] == ',' ){
                     record_d = ';' ; i=2; global++;
                  }
                  else{
                     record_d = Global[3] ; i=5; global++;
                  }
               }
               else
               {
                  field_d=Global[2] ; global++;
                  if (Global[4] != field_d){
                     record_d=Global[6] ; i = 8; global++;
                  }
                  else{
                     record_d=';'; i = 5; global++;
                  }
               }
            }
            while(i<pos && Global[i] != ' '){
               ii=i;
               while(Global[i] != field_d && Global[i] != record_d && i<pos &&
                     Global[i] != ' '){
                 if (Global[i] == 'H'){
                    temp[0] = Global[ii];
                    if (ii+1 == i)
                      temp[1] = '\0';
                    else
                      temp[1] = Global[ii+1];
                    ii=i+1;
                    i += atoi(temp)+1;
                    break;
                 }
                 i++;
               }
               for (j=ii;j<i;j++) 
               {
                 string[j-ii] = Global[j];
               }
               string[j-ii] = '\0';
               if (PrintOut == 1) printf("%d: %s\n",global,string);
               switch(global){
                  case 13: /*Scale factor*/
                    if (isdigit(string[0]))
                    {
                      Scale = atof(string);
                    }
                    break;
                  case 14: /*Unit flag*/
                    if (isdigit(string[0]))
                    {
                      Unit = atoi(string);
                      if (Unit == 3) Unit = 1;
                      if (Unit > 3) Unit--;
                    }
                    break;
                  case 19: /*Tolerence*/
                    if (isdigit(string[0]))
                    {
                      for(j=0;j<strlen(string);j++)
                      {
                         if (string[j] == 'D' || string[j] == 'd')
                            string[j] = 'E';
                      }
                      Tolerence = atof(string);
                    }
                    break;
                  case 20: /*Bound*/
                    if (isdigit(string[0]))
                    {
                      Bound = atof(string);
                    }
                    break;
               }
               global++,i++;
            }
            free(Global);
            break ;
        case 'D':
            if (PrintOut == 1) fprintf(stdout,".");
            fflush(stdout);
            max_seqnum = (int)field_num(buf,10);
            if (fwrite(buf,sizeof(char),RECBUF,dfp) != RECBUF) dstat++;
            ddata++;
            sp = fgets(buf, RECBUF+c_shift, fp);
            break;
        case 'P':
            if (fwrite(buf,sizeof(char),RECBUF,pfp) != RECBUF) pstat++;
            sp = fgets(buf, RECBUF+c_shift, fp);
            break;
        case 'T':
            sp = 0;
            break;                    
        default:
            fprintf(stdout, "Cannot resolve IGES record:\n%s", buf);
            return 0;
        }
    }
    if (PrintOut == 1) fprintf(stdout,"\n"); fflush(stdout);

    fclose(fp); 

    if (dstat) {
       Error(routine,"Error writing Dfile");
       sprintf(string," Error occured writing Dfile\n");
       gui_add_to_message_buffer_(&info,string,strlen(string));
       return 0;
    }

    if (pstat) {
       Error(routine,"Error writing Pfile");
       sprintf(string," Error occured writing Pfile\n");
       gui_add_to_message_buffer_(&info,string,strlen(string));
       return 0;
    }

    *scale = Scale;
    *units = Unit;
    *tolerence = Tolerence;
    bound = Bound;

    return(ddata/2);
}

DirEntry_t *create_entry(int type, int id, void *ndata)
{
    DirEntry_t *DE;
    char *routine="create_entry";

    DE = (DirEntry_t *)malloc(sizeof(DirEntry_t));
    if (DE == NULL) {
       Error("create_entry","Allocation Failed");
       return (NULL);
    }

    DE->id = id;
    DE->entype = (int)entype;
    DE->parent_entype = 0;
    DE->seqnum = seqnum;
    DE->level = (int)level;
    DE->process = process;
    DE->visible = (int)visible;
    DE->label = name;
    DE->color = (int)Color;
    DE->red = red;
    DE->green = green;
    DE->blue = blue;
    DE->type = type;
    DE->data  = NULL;
    DE->ndata = ndata;
    DE->next = NULL;

    name = NULL;

    if (PrintOut == 1)
    {
      if (DE->label != NULL)
        printf(" ...created DE %d: num=%d lbl=%s vis=%d level=%d\n",
               entype,seqnum,DE->label,visible,level);
      else
        printf(" ...created DE %d: num=%d lbl=NONE vis=%d level=%d\n",
               entype,seqnum,visible,level);
    }

    if (DE_array != NULL)
    {
       if (seqnum > max_seqnum)
       {
          Error(routine,"Attempted to exceed array bounds");
          return NULL;
       }
       DE_array[seqnum] = DE;
    }
    else if (PrintOut == 1)
       printf(" ...Directory array is NULL\n");

    return(DE);
}

DirEntry_t *find_entry(long seq_num, int echo)
{
    DirEntry_t *DE;
    char *routine = "find_entry";

    if (PrintOut == 1 && echo == 1)
       printf(" ...searching for seqnum %d\n",seq_num);

    if (seq_num < 1 || seq_num > max_seqnum)
    {
       Error(routine,"Attempted to exceed array bounds");
       return NULL;
    }

    DE = DE_array[seq_num];

    if (DE != NULL)
       return(DE);
    else if (PrintOut == 1 && echo == 1)
       fprintf(stdout,"***ERROR: Could not find Directory Entry data***\n");

    return(NULL);
}

void entity_process( menuCalldata *info, int maxdata )
{
    char buf[RECBUF];
    int  i,j,vis;
    int ccurv_id,pcurv_id;
    int nsurf_id,ncurv_id,ruled_id,rotate_id,cylinder_id,psurf_id;
    int point_id,line_id,group_id;
    int pid=0, lid=0, sid=0;
    NurbCurv **curves;
    char message[100];

    long *pp = NULL,*ncp = NULL,*nsp = NULL,*gp = NULL;
    NurbCurv  *nc = NULL;
    NurbSurf  *ns = NULL;
    Point     *P = NULL;
    void      **VP = NULL;

    time_t st,ft;
    double dt;

    int istat;
    int resnum, res;

    DirEntry_t *DE;

    char *routine = "entity_process";

    if (info == NULL){
       Error(routine,"Error info is NULL");
       return;
    }

    pp = (long *)malloc(maxdata*sizeof(long));
    ncp = (long *)malloc(maxdata*sizeof(long));
    nsp = (long *)malloc(maxdata*sizeof(long));
    gp = (long *)malloc(maxdata*sizeof(long));

    VP = (void **)malloc(maxdata*sizeof(void *));
   
    if(VP == NULL || pp == NULL || ncp == NULL || nsp == NULL || gp == NULL){
      Error(routine,"Allocation Failed");
      if(VP) free(VP);
      if(pp) free(pp);
      if(ncp) free(ncp);
      if(nsp) free(nsp);
      if(gp) free(gp);
      return;
    }

    if (max_seqnum != 0)
    {
      DE_array = (DirEntry_t **)malloc((max_seqnum+1)*sizeof(DirEntry_t *));
      if (DE_array == NULL)
      {
         Error(routine,"Allocation Failed");
         if(VP) free(VP);
         if(pp) free(pp);
         if(ncp) free(ncp);
         if(nsp) free(nsp);
         if(gp) free(gp);
         max_seqnum = 0;
         return;
      }
      for (i=0;i<=max_seqnum;i++)
         DE_array[i] = NULL;
    }

/* the process we have right now is : let the CAGI process the */
/* points , line ( include the curves ) first , then the surface */
/* because some input files put the entity type 120 which is     */
/* body of revolution first , the rotation axex and the boundary */
/* latter -- this is against the nature habit , but we have noway */
/* to change it , therefor we do the process twice -- waste time ,*/
/* but no choice right now                                        */


    /*========  Process "D" & "P" files  ========*/
    rewind(dfp); rewind(pfp);

    ccurv_id = line_id = pcurv_id = 0;
    ruled_id = rotate_id = cylinder_id = psurf_id = 0;

    point_id = ncurv_id = nsurf_id = group_id = 0;

    st = time(NULL);

    while (fread(buf,sizeof(char),RECBUF,dfp))
    {
      entype    = field_num(buf,1); 
      parameter = field_num(buf,2);
      level     = field_num(buf,5);
      trans_mtr = field_num(buf,7);
      status    = field_num(buf,9);
      seqnum    = field_num(buf,10);
      fread(buf,sizeof(char),RECBUF,dfp);
      Color     = field_num(buf,3);
      form      = field_num(buf,5);
      savep     = ftell(dfp);
      if (PrintOut == 1)
      {
         if (trans_mtr)
            printf("*");
         else
            printf(" ");
      }
      visible     = status/1000000L;
      subordinate = status/10000L-visible*100L;
      entity_use  = status/100L-visible*10000L-subordinate*100L;
      hierarchy   = status-100L*(status/100L);
      process = (int)entity_use;
      if (process == 5) process = 0;
      if (PrintOut == 1) printf("Entity=%d",entype);
      switch (entype) 
      {
        case   0: /* NULL */
          if (PrintOut == 1) printf(": NULL\n");
          break;
        case 100: /* Circular Arc */
          if (PrintOut == 1) printf(": Circular Arc\n");
          if ((nc = ins_100()) != NULL){
             DE = create_entry(NURB_CURVE,ncurv_id,(void *)nc);
             if(DE == NULL){
               Error(routine,"create_entry failed");
               break;
             } 
             ncp[ncurv_id] = seqnum;
             ncurv_id++;
          }
          break;
        case 102: /* Composite Curve */
          if (PrintOut == 1) printf(": Composite Curve\n");
          if ((nc = ins_102()) != NULL){
             DE = create_entry(NURB_CURVE,ncurv_id,(void *)nc);
             if (DE == NULL) {
                Error(routine,"crate_entry failed");
                break;
             }
             ncp[ncurv_id] = seqnum;
             ncurv_id++;
          }
          break;
        case 104: /* Conic Arc */
          if (PrintOut == 1) printf(": Conic Arc\n");
          if ((nc = ins_104()) != NULL){
             DE = create_entry(NURB_CURVE,ncurv_id,(void *)nc);
             if (DE == NULL){
                Error(routine,"create_entry failed");
                break;
             }
             ncp[ncurv_id] = seqnum;
             ncurv_id++;
          }
          break;
        case 106:
          switch(form){
             case 20:
             case 21:
                if (PrintOut == 1) printf(": Centerline\n");
                break;
             case 31:
             case 32:
             case 33:
             case 34:
             case 35:
             case 36:
             case 37:
             case 38:
                if (PrintOut == 1) printf(": Section\n");
                break;
             case 40:
                if (PrintOut == 1) printf(": Witness Line\n");
                break;
             default:
                if (PrintOut == 1) printf(": Copious Data form %d\n",form);
                pid = lid = 0;
                ins_106(info,&pid,&lid,VP);
                if (pid) {
                   for (i=0;i<pid;i++){
                      if (P = (Point *)VP[i]){
                         DE = create_entry(POINT,point_id,(void *)P);
                         if (DE == NULL) {
                            Error(routine,"create_entry failed");
                            break;
                         }
                         pp[point_id] = seqnum;
                         point_id++;
                      }
                   }
                }
                if (lid) {
                   for (i=0;i<lid;i++){
                      if (nc = (NurbCurv *)VP[i+pid]){
                         DE = create_entry(NURB_CURVE,ncurv_id,(void *)nc);
                         if (DE == NULL) {
                            Error(routine,"create_entry failed");
                            break;
                         }
                         ncp[ncurv_id] = seqnum;
                         ncurv_id++;
                      }
                   }
                }
                break;
          }
          break;
        case 108: /* Plane  */
          if (PrintOut == 1) printf(": Plane form %d\n",form);
          if ((ns == ins_108()) != NULL){
             DE = create_entry(NURB_SURFACE,nsurf_id,(void *)ns);
             if (DE == NULL) {
                Error(routine,"create_entry failed");
                break;
             }
             nsp[nsurf_id] = seqnum;
             nsurf_id++;
          }
          break;
        case 110: /* Line */
          if (PrintOut == 1) printf(": Line\n");
          if ((nc = ins_110(ncurv_id+1)) != NULL){
             DE = create_entry(NURB_CURVE,ncurv_id,(void *)nc);
             if (DE == NULL) {
                Error(routine,"create_entry failed");
                break;
             }
             ncp[ncurv_id] = seqnum;
             ncurv_id++;
          }
          break;
        case 112:/* Parametric Spline Curve */
          if (PrintOut == 1) printf(": Parametric Spline Curve\n");
          if ((nc = ins_112()) != NULL){
             DE = create_entry(NURB_CURVE,ncurv_id,(void *)nc);
             if (DE == NULL) {
                Error(routine,"create_entry failed");
                break;
             }
             ncp[ncurv_id] = seqnum;
             ncurv_id++;
          }
          break;
        case 114:/* Parametric Spline Surface */
          if (PrintOut == 1) printf(": Parametric Spline Surface\n");
          if ((ns = ins_114()) != NULL){
             DE = create_entry(NURB_SURFACE,nsurf_id,(void *)ns);
             if (DE == NULL) {
                Error(routine,"create_entry failed");
                break;
             }
             nsp[nsurf_id] = seqnum;
             nsurf_id++;
          }
          break;
        case 116: /* Point */
          if (PrintOut == 1) printf(": Point\n");
          if ((P = ins_116()) != NULL){
             DE = create_entry(POINT,point_id,(void *)P);
             if (DE == NULL) {
                Error(routine,"create_entry failed");
                break;
             }
             pp[point_id] = seqnum;
             point_id++;
          }
          break;
        case 118: /* Ruled Surface */
          if (PrintOut == 1) printf(": Ruled Surface\n");
          if ((ns = ins_118()) != NULL){
             DE = create_entry(NURB_SURFACE,nsurf_id,(void *)ns);
             if( DE == NULL) {
               Error(routine,"create_entry failed");
               break;
             }
             nsp[nsurf_id] = seqnum;
             nsurf_id++;
          }
          break;
        case 120: /* Surface of Revolution */
          if (PrintOut == 1) printf(": Surface of Revolution\n");
          if ((ns = ins_120()) != NULL){
             DE = create_entry(NURB_SURFACE,nsurf_id,(void *)ns);
             if (DE == NULL) {
                Error(routine,"create_entry failed");
                break;
             }
             nsp[nsurf_id] = seqnum;
             nsurf_id++;
          }
          break;
        case 122: /*  for tabulated cylinder */
          if (PrintOut == 1) printf(": Tabulated Cylinder\n");
          if ((ns = ins_122()) != NULL){
             DE = create_entry(NURB_SURFACE,nsurf_id,(void *)ns);
             if( DE == NULL) {
               Error(routine,"create_entry failed");
               break;
             }
             nsp[nsurf_id] = seqnum;
             nsurf_id++;
          }
          break;  
        case 123: /* Direction */
          if (PrintOut == 1) printf(": Direction\n");
          break;  
        case 124: /* Transformation Matrix */
          if (PrintOut == 1) printf(": Transformation Matrix\n");
          break;  
        case 125: /* Flash */
          if (PrintOut == 1) printf(": Flash\n");
          if ((curves = ins_125()) != NULL){
             if ((nc = curves[0]) != NULL){
                DE = create_entry(NURB_CURVE,ncurv_id,(void *)nc);
                if (DE == NULL) {
                   Error(routine,"create_entry failed");
                   break;
                }
                ncp[ncurv_id] = seqnum;
                ncurv_id++;
             }
             if ((nc = curves[1]) != NULL){
                DE = create_entry(NURB_CURVE,ncurv_id,(void *)nc);
                if (DE == NULL) {
                   Error(routine,"create_entry failed");
                   break;
                }
                ncp[ncurv_id] = seqnum;
                ncurv_id++;
             }
          }
          break;  
        case 126: /* Rational B-Spline Curve */
          if (PrintOut == 1) printf(": Rational B-Spline Curve\n");
          if ((nc = ins_126(ncurv_id+1)) != NULL){
             DE = create_entry(NURB_CURVE,ncurv_id,(void *)nc);
             if (DE == NULL) {
                Error(routine,"create_entry failed");
                break;
             }
             ncp[ncurv_id] = seqnum;
             ncurv_id++;
          }
          break;  
        case 128: /* Rational B-Spline Surface */
          if (PrintOut == 1) printf(": Rational B-Spline Surface\n");
          if ((ns = ins_128(nsurf_id+1)) != NULL){
             DE = create_entry(NURB_SURFACE,nsurf_id,(void *)ns);
             if (DE == NULL) {
                Error(routine,"create_entry failed");
                break;
             }
             nsp[nsurf_id] = seqnum;
             nsurf_id++;
          }
          break;
        case 130: /* Offset Curve */
          if (PrintOut == 1) printf(": Offset Curve\n");
          if ((nc = ins_130()) != NULL){
             DE = create_entry(NURB_CURVE,ncurv_id,(void *)nc);
             if (DE == NULL) {
                Error(routine,"create_entry failed");
                break;
             }
             ncp[ncurv_id] = seqnum;
             ncurv_id++;
          }
          break;  
        case 132: /* Connect Point */
          if (PrintOut == 1) printf(": Connect Point\n");
          break;  
        case 134: /* Node */
          if (PrintOut == 1) printf(": Node\n");
          break;  
        case 136: /* Finite Element */
          if (PrintOut == 1) printf(": Finite Element\n");
          break;  
        case 138: /* Nodal Displacement and Rotation */
          if (PrintOut == 1) printf(": Nodal Displacement and Rotation\n");
          break;  
        case 140: /* Offset Surface */
          if (PrintOut == 1) printf(": Offset Surface\n");
          if ((ns = ins_140()) != NULL){
             DE = create_entry(NURB_SURFACE,nsurf_id,(void *)ns);
             if (DE == NULL) {
                Error(routine,"create_entry failed");
                break;
             }
             nsp[nsurf_id] = seqnum;
             nsurf_id++;
          }
          break;
        case 141: /* Boundary */
          if (PrintOut == 1) printf(": Boundary\n");
          break;
        case 142: /* Curve on a Parametric Surface */
          if (PrintOut == 1) printf(": Curve on a Parametric Surface\n");
          if ((nc = ins_142()) != NULL){
             DE = create_entry(PARAMETRIC_CURVE,ncurv_id,(void *)nc);
             if(DE == NULL) {
               Error(routine,"create_entry failed");
               break;
             }
             ncp[ncurv_id] = seqnum;
             ncurv_id++;
          }
          break;
        case 143: /* Bounded Surface */
          if (PrintOut == 1) printf(": Bounded Surface\n");
          break;
        case 144: /* Trimmed (Parametric) Surface */
          if (PrintOut == 1) printf(": Trimmed (Parametric) Surface\n");
          if (Process != 4)
          {
#if 0
             if ((ns = ins_144(info)) != NULL)
             {
                DE = create_entry(PARAMETRIC_SURFACE,nsurf_id,(void *)ns);
                if(DE == NULL) {
                  Error(routine,"create_entry failed");
                  break;
                }
                nsp[nsurf_id] = seqnum;
                nsurf_id++;
             }
#endif
          }
          break;
        case 146: /* Nodal Results */
          if (PrintOut == 1) printf(": Nodal Results\n");
          break;
        case 148: /* Element Results */
          if (PrintOut == 1) printf(": Element Results\n");
          break;
        case 150: /* Block */
          if (PrintOut == 1) printf(": Block\n");
          break;
        case 152: /* Right Angular Wedge */
          if (PrintOut == 1) printf(": Right Angular Wedge\n");
          break;
        case 154: /* Right Circular Cylinder */
          if (PrintOut == 1) printf(": Right Circular Cylinder\n");
          break;
        case 156: /* Right Circular Cone Frustrum */
          if (PrintOut == 1) printf(": Right Circular Cone Frustrum\n");
          break;
        case 158: /* Sphere */
          if (PrintOut == 1) printf(": Sphere\n");
          break;
        case 160: /* Torus */
          if (PrintOut == 1) printf(": Torus\n");
          break;
        case 162: /* Solid of Revolution */
          if (PrintOut == 1) printf(": Solid of Revolution\n");
          break;
        case 164: /* Solid of Linear Extrusion */
          if (PrintOut == 1) printf(": Solid of Linear Extrusion\n");
          break;
        case 168: /* Ellipsoid */
          if (PrintOut == 1) printf(": Ellipsoid\n");
          break;
        case 180: /* Boolean Tree */
          if (PrintOut == 1) printf(": Boolean Tree\n");
          break;
        case 182: /* Selected Component */
          if (PrintOut == 1) printf(": Selected Component\n");
          break;
        case 184: /* Solid Assembly */
          if (PrintOut == 1) printf(": Solid Assembly\n");
          break;
        case 186: /* Manifold Solid B-Rep Object */
          if (PrintOut == 1) printf(": Manifold Solid B-Rep Object\n");
          break;
        case 190: /* Plane Surface */
          if (PrintOut == 1) printf(": Plane Surface\n");
          break;
        case 192: /* Right Circular Cylindrical Surface */
          if (PrintOut == 1) printf(": Right Circular Cylindrical Surface\n");
          break;
        case 194: /* Right Circular Conical Surface */
          if (PrintOut == 1) printf(": Right Circular Conical Surface\n");
          break;
        case 196: /* Spherical Surface */
          if (PrintOut == 1) printf(": Spherical Surface\n");
          break;
        case 198: /* Toroidal Surface */
          if (PrintOut == 1) printf(": Toroidal Surface\n");
          break;
        case 202: /* Angular Dimension */
          if (PrintOut == 1) printf(": Angular Dimension\n");
          break;
        case 204: /* Curve Dimension */
          if (PrintOut == 1) printf(": Curve Dimension\n");
          break;
        case 206: /* Diameter Dimension */
          if (PrintOut == 1) printf(": Diameter Dimension\n");
          break;
        case 208: /* Flag Note */
          if (PrintOut == 1) printf(": Flag Note\n");
          break;
        case 210: /* General Label */
          if (PrintOut == 1) printf(": General Label\n");
          break;
        case 212: /* General Note */
          if (PrintOut == 1) printf(": General Note\n");
          break;
        case 213: /* New General Note */
          if (PrintOut == 1) printf(": New General Note\n");
          break;
        case 214: /* Leader (Arrow) */
          if (PrintOut == 1) printf(": Leader (Arrow)\n");
          break;
        case 216: /* Linear Dimension */
          if (PrintOut == 1) printf(": Linear Dimension\n");
          break;
        case 218: /* Ordinate Dimension */
          if (PrintOut == 1) printf(": Ordinate Dimension\n");
          break;
        case 220: /* Point Dimension */
          if (PrintOut == 1) printf(": Point Dimension\n");
          break;
        case 222: /* Radius Dimension */
          if (PrintOut == 1) printf(": Radius Dimension\n");
          break;
        case 228: /* General Symbol */
          if (PrintOut == 1) printf(": General Symbol\n");
          break;
        case 230: /* Sectioned Area */
          if (PrintOut == 1) printf(": Sectioned Area\n");
          break;
        case 302: /* Associativity Definition */
          if (PrintOut == 1) printf(": Associativity Definition\n");
          break;
        case 304: /* Line Font Definition */
          if (PrintOut == 1) printf(": Line Font Definition\n");
          break;
        case 308: /* Subfigure Definition */
          if (PrintOut == 1) printf(": Subfigure Definition\n");
          break;
        case 310: /* Text Font Definition */
          if (PrintOut == 1) printf(": Text Font Definition\n");
          break;
        case 312: /* Text Display Template */
          if (PrintOut == 1) printf(": Text Display Template form %d\n",form);
          break;
        case 314: /* Color Definition */
          if (PrintOut == 1) printf(": Color Definition\n");
          break;
        case 316: /* Units Data */
          if (PrintOut == 1) printf(": Units Data\n");
          break;
        case 320: /* Network Subfigure Definition */
          if (PrintOut == 1) printf(": Network Subfigure Definition\n");
          break;
        case 322: /* Attribute Table Definition */
          if (PrintOut == 1) printf(": Attribute Table Definition\n");
          break;
        case 402: /* Associative Instance */
          if (PrintOut == 1) printf(": Associative Instance form %d\n",form);
          if (form == 1 || form == 7 || form == 14 || form == 15){
             DE = create_entry(GROUP,group_id,NULL);
             if (DE == NULL) {
                Error(routine,"create_entry failed");
                break;
             }
             gp[group_id] = seqnum;
             group_id++;
          }
          break;
        case 404: /* Drawing */
          if (PrintOut == 1) printf(": Drawing\n");
          break;
        case 406: /* Property */
          if (PrintOut == 1) printf(": Property form %d\n",form);
          break;
        case 408: /* Singular Subfigure Instance */
          if (PrintOut == 1) printf(": Singular Subfigure Instance\n");
          break;
        case 410: /* View */
          if (PrintOut == 1) printf(": View form %d\n",form);
          break;
        case 412: /* Rectangular Array Subfigure Instance */
          if (PrintOut == 1) printf(": Rectangular Array Subfigure Instance\n");
          break;
        case 414: /* Circular Array Subfigure Instance */
          if (PrintOut == 1) printf(": Circular Array Subfigure Instance\n");
          break;
        case 416: /* External Reference */
          if (PrintOut == 1) printf(": External Reference\n");
          break;
        case 418: /* Nodal Load/Constraint */
          if (PrintOut == 1) printf(": Nodal Load/Constraint\n");
          break;
        case 420: /* Network Subfigure Instance */
          if (PrintOut == 1) printf(": Network Subfigure Instance\n");
          break;
        case 422: /* Attribute Table Instance */
          if (PrintOut == 1) printf(": Attribute Table Instance\n");
          break;
        case 430: /* Solid Instance */
          if (PrintOut == 1) printf(": Solid Instance\n");
          break;
        case 502: /* Vertex */
          if (PrintOut == 1) printf(": Vertex\n");
          break;
        case 504: /* Edge */
          if (PrintOut == 1) printf(": Edge\n");
          break;
        case 508: /* Loop */
          if (PrintOut == 1) printf(": Loop\n");
          break;
        case 510: /* Face */
          if (PrintOut == 1) printf(": Face\n");
          break;
        case 5001: /* discrete data */
          if (PrintOut == 1) printf(": discrete data\n");
          lid = sid = 0;
          ins_5001(&lid,&sid,ncurv_id,nsurf_id,VP);
          if (lid) {
             if (nc = (NurbCurv *)VP[0]){
                DE = create_entry(NURB_CURVE,ncurv_id,(void *)nc); 
                if (DE == NULL) {
                   Error(routine,"create_entry failed");
                   break;
                }
                ncp[ncurv_id] = seqnum;
                ncurv_id++;
             }
          }
          if (sid) {
             if (ns = (NurbSurf *)VP[0]){
                DE = create_entry(NURB_SURFACE,nsurf_id,(void *)ns);
                if (DE == NULL) {
                   Error(routine,"create_entry failed");
                   break;
                }
                nsp[nsurf_id] = seqnum;
                nsurf_id++;
             }
          }
          break; 
        case 7366: /* parametric surface */
          if (PrintOut == 1) printf(": Parametric Surface\n");
          if ((ns = ins_7366()) != NULL)
          {
             DE = create_entry(NURB_SURFACE,nsurf_id,(void *)ns);
             if (DE == NULL) {
                Error(routine,"create_entry failed");
                break;
             }
             nsp[nsurf_id] = seqnum;
             nsurf_id++;
          }
          break;
        default:
          if (PrintOut == 1) printf(": *** not recognized ***\n");
          break;
      }
    }

    ft = time(NULL);
    dt = difftime(ft,st);

    fprintf(stdout,"\n>>> END READING IGES DATA (%g seconds) <<<<\n\n",dt);

    rewind(dfp); rewind(pfp);

    AddLevel(info,info->data->current_level);

    /* Determine if curve resolution should be reduced */
    resnum = 140;
    gui_get_int_resource_(&info,&resnum,&res);
    if (ncurv_id > 499 && res > 25)
    {
      res = 2*res/3;
      if (ncurv_id > 999 && res > 25)
        res = 2*res/3;
      if (res < 25) res = 25;
      gui_set_int_resource(info,resnum,res);
    }

    /* Determine if surface resolution should be reduced */
    resnum = 100;
    gui_get_int_resource_(&info,&resnum,&res);
    if (nsurf_id > 249 && res > 5)
    {
      res = 2*res/3;
      if (nsurf_id > 499 && res > 5)
        res = 2*res/3;
      if (res < 5) res = 5;
      gui_set_int_resource(info,resnum,res);
    }

    if (point_id)
    {
      Vertex *vertex;
      st = time(NULL);
      sprintf(message,"Possible Points %d",point_id);
      gui_add_to_message_buffer_(&info,message,strlen(message));
      fprintf(stdout," %s\n",message);
      for (vis=j=i=0; i < point_id; i++)
      {
        DE = find_entry(pp[i],0);
        if (DE == NULL || DE->type != POINT) continue;
        if ((P = (Point *)DE->ndata) != NULL &&
            (Process == 0 || Process >= 3))
        {
          if (DE->process == 0)
          {
            if ((vertex = AddPointAtLevel(info,P->x,P->y,P->z,DE->level,
                                          &istat)) == NULL || istat != NO_ERROR)
              Error(routine,"AddPoint failed");
            if (DE->visible != 0)
               vertex->draw_props = INVISIBLE;
            else
               vis++;
            fprintf(stdout,"."); fflush(stdout);
            if (DE->label) vertex->name = DE->label;
            DE->type = VERTEX;
            DE->data = (void *)vertex;
            DE->ndata = NULL;
          }
          free(P); P = NULL;
          DE->ndata = NULL;
          j++;
        }
        else
        {
          if (Process == 1 || Process == 2)
          {
            if (P != NULL) free(P); P = NULL;
            DE->ndata = NULL;
          }
        }
      }
      sprintf(message,"%d processed %d visible",j,vis);
      gui_add_to_message_buffer_(&info,message,strlen(message));
      ft = time(NULL);
      dt = difftime(ft,st);
      sprintf(message,"%d processed %d visible (%g seconds)",j,vis,dt);
      fprintf(stdout,"%s\n",message);
    }

    if (ncurv_id)
    {
      Edge *edge;
      st = time(NULL);
      sprintf(message,"Possible NURBS Curves %d",ncurv_id);
      gui_add_to_message_buffer_(&info,message,strlen(message));
      fprintf(stdout," %s\n",message);
      for (vis=j=i=0; i < ncurv_id; i++)
      {
        DE = find_entry(ncp[i],0);
        if (DE == NULL || 
            !(DE->type == NURB_CURVE || DE->type == PARAMETRIC_CURVE)) continue;
        if ((nc = (NurbCurv *)DE->ndata) != NULL && nc->edge_ptr == NULL)
        {
          nc->Delete = 0;
          if (DE->process == 0 && (Process <= 1 || Process >= 3))
          {
            if (nc->parent_surf_ptr != NULL)
               NormalizeSurfaceDomain(nc->parent_surf_ptr);
            if ((edge = AddCurveAtLevel(info,nc,0,NURB_CURVE,DE->level,Glue))
                 == NULL)
            {
               if (DE->parent_entype)
                 printf(">>>>>>>>>>parent entity = %d\n",DE->parent_entype);
               printf("entype = %d seqnum = %d\n",DE->entype,DE->seqnum);
               Error(routine,"AddCurve failed");
               FreeNurbCurv(&nc);
               DE->process = 1;
               continue;
            }
            if (DE->visible != 0)
               set_edge_object(edge,INVISIBLE);
            else
               vis++;
            fprintf(stdout,"."); fflush(stdout);
            if (DE->label) edge->name = DE->label;
            DE->type = EDGE;
            DE->data = (void *)edge;
            j++;
          }
          else
          {
            if (nc != NULL)
            {
              if (DE->visible != 2 || Process == 2)
              {
                if (nc->parent_surf_ptr != NULL)
                {
                  DeleteFromChildList(nc->parent_surf_ptr,(void *)nc);
                  nc->parent_surf_ptr = NULL;
                }
                nc->edge_ptr = NULL;
                FreeNurbCurv(&nc);
                DE->ndata = NULL;
              }
              else
              {
                NurbSurf *forms = nc->parent_surf_ptr;
                if (forms != NULL)
                  AddToChildList(forms,(void *)nc,NURB_CURVE);
                ns = forms;
                if (ns->Delete != 0) ns->face_ptr = NULL;
                forms = ns->parent_surf_ptr;
                while (forms != NULL)
                {
                  if (forms->Delete != 0) ns->face_ptr = NULL;
                  AddToChildList(forms,(void *)ns,NURB_SURFACE);
                  ns = forms;
                  forms = ns->parent_surf_ptr;
                }
                nc->edge_ptr = NULL; 
                nc->Delete = 1;
              }
            }
          }
        }
      }
      sprintf(message,"%d processed %d visible",j,vis);
      gui_add_to_message_buffer_(&info,message,strlen(message));
      ft = time(NULL);
      dt = difftime(ft,st);
      sprintf(message,"%d processed %d visible (%g seconds)",j,vis,dt);
      fprintf(stdout,"%s\n",message);
    }

    if (nsurf_id)
    {
      Face *face;
      st = time(NULL);
      sprintf(message,"Possible NURBS Surfaces %d",nsurf_id);
      gui_add_to_message_buffer_(&info,message,strlen(message));
      fprintf(stdout," %s\n",message);
      for (vis=j=i=0; i < nsurf_id; i++)
      {
        DE = find_entry(nsp[i],0);
        if (DE == NULL || 
            !(DE->type == NURB_SURFACE ||
              DE->type == PARAMETRIC_SURFACE)) continue;
        if ((ns = (NurbSurf *)DE->ndata) != NULL && ns->face_ptr == NULL)
        {
          ns->Delete = 0;
          if (DE->process == 0 && DE->visible != 2 &&
              (Process == 0 || Process >= 2)){
            if ((face = 
                 AddSurfaceAtLevel(info,ns,0,0,NURB_SURFACE,DE->level,Glue))
                 == NULL)
            {
               printf("entype = %d seqnum = %d\n",DE->entype,DE->seqnum);
               Error(routine,"AddSurface failed");
               FreeNurbSurf(&ns);
               DE->ndata = NULL;
               continue;
            }
            if (DE->visible != 0)
               set_face_object(face,INVISIBLE);
            else
               vis++;
            fprintf(stdout,"."); fflush(stdout);
            if (DE->label) face->name = DE->label;
            DE->type = FACE;
            DE->data = (void *)face;
            j++;
          }
          else
          {
            if (ns != NULL)
            {
              NurbSurf *forms = ns->parent_surf_ptr;
              if (DE->visible != 2 || Process == 1)
              {
                ns->face_ptr = NULL;
                ns->Delete = 1;
                DE->ndata = NULL;
              }
              else
              {
                ns->face_ptr = NULL;
                ns->Delete = 1;
              }
              while (forms != NULL)
              {
                if (forms->Delete != 0) forms->face_ptr = NULL;
                AddToChildList(forms,(void *)ns,NURB_SURFACE);
                ns = forms;
                forms = ns->parent_surf_ptr;
              }
            }
          }
        }
      }
      sprintf(message,"%d processed %d visible",j,vis);
      gui_add_to_message_buffer_(&info,message,strlen(message));
      ft = time(NULL);
      dt = difftime(ft,st);
      sprintf(message,"%d processed %d visible (%g seconds)",j,vis,dt);
      fprintf(stdout,"%s\n",message);
    }

    if (group_id)
    {
      Group *group;
      st = time(NULL);
      sprintf(message,"Possible Groups %d",group_id);
      gui_add_to_message_buffer_(&info,message,strlen(message));
      fprintf(stdout," %s\n",message);
      for (vis=j=i=0; i<group_id; i++)
      {
        group = NULL;
        seqnum = gp[i];
        DE = find_entry(seqnum,0);
        if (DE == NULL || DE->type != GROUP) continue;
        if (DE->process == 0 && Process == 0)
        {
          if ((group = ins_402(info)) == NULL)
          {
             Error(routine,"Grouping failed");
             continue;
          }
          if (group->name != NULL &&
              strstr(group->name,"Carpeted Surfaces") != NULL)
          {
             info->data->CarpetSurfaces = group;
          }
          if (DE->visible != 0)
             group->draw_props = INVISIBLE;
          else
             vis++;
          fprintf(stdout,"."); fflush(stdout);
          if (DE->label) group->name = DE->label;
          DE->data = group;
          group->object_changed = 1;
          j++;
        }
      }
      UpdatePropsObjects(info);
      sprintf(message,"%d processed %d visible",j,vis);
      gui_add_to_message_buffer_(&info,message,strlen(message));
      ft = time(NULL);
      dt = difftime(ft,st);
      sprintf(message,"%d processed %d visible (%g seconds)",j,vis,dt);
      fprintf(stdout,"%s\n",message);
    }

    fprintf(stdout,"\n>>> END PROCESSING IGES DATA <<<<\n");

    if (VP) free(VP);
    if (pp) free(pp);
    if (ncp) free(ncp); 
    if (nsp) free(nsp);
    if (gp) free(gp);
    if (DE_array != NULL)
    {
      for(i=0;i<=max_seqnum;i++)
         if (DE_array[i] != NULL) free(DE_array[i]);
      free (DE_array);
      DE_array = NULL;
    }

    max_seqnum = 0;

    fclose(dfp);
    fclose(pfp);

    dfp = NULL;
    pfp = NULL;

    return;
}

long field_num(char *buff, int field) /* get the field entry of the Directory File line */
                       
{
    int i,j; char tmp[8]; int index;

    index = (field-1)*8;
    for (i=index,j=0;j<8;i++,j++){
        tmp[j] = buff[i];      
        if (tmp[j] == 'D' || tmp[j] == 'd') tmp[j]=' ';
    }
    return(atoi(tmp));
} 

int perf_pro(double *data, int max_data)
{
    char test[RECBUF],temp[RECBUF];
    int  count=0,end_rec=0,i=0,j=0,k=0;

    char *routine = "perf_pro";

    if (data == NULL){
       Error(routine,"Data pointer is NULL");
       return max_data;
    }

    if (max_data <= 0){
       Error(routine,"trying to read <= 0 data");
       return max_data;
    }

    while ( end_rec == 0 )
    {
      fread(test,sizeof(char),RECBUF,pfp); j = 0;

      if (test[0] == record_d)
      {
         i = 0;
         while(test[i] == record_d && i<65)
         {
            data[count] = 0.;
            if (ngp_debug()-1000 == entype)
            {
              fprintf(stdout,"%d of %d: %.16lg\n",
                      count+1,max_data,data[count]);
              fflush(stdout);
            }
            count++;
            if (count == max_data) return(0);
            i++;
         }
      }
      else
      {
         for (i=0;i<65;i++)
         {
           if ((test[i] != field_d) && (test[i] != record_d ))
           {
             if (test[i] == 'D' || test[i] == 'd' || test[i] == 'E') 
               test[i] = 'e';
             temp[j]=test[i];
             j++;
           }
           else
           {
             temp[j] = '\0'; j = 0;
             data[count] = atof(temp);
             if (ngp_debug()-1000 == entype)
             {
               fprintf(stdout,"%d of %d: %.16lg (%s)\n",
                       count+1,max_data,data[count],temp);
               fflush(stdout);
             }
             temp[0] = '\0';
             if (test[i] == record_d) end_rec = 1;
             count++;
             if (count == max_data) return(0);
           }
         }
      }
    }
    return(max_data-count);
}

int get_matrix(double RotMat[3][3], double Trans[3])
{
    long ptr;
    char buff[RECBUF];
    double *data = NULL;
    char *routine = "get_matrix";

    if (trans_mtr != 0){
       fseek(dfp,(trans_mtr - 1)*RECBUF,0);    
       fread(buff,sizeof(char),RECBUF,dfp);
       ptr = field_num(buff,2);
       fread(buff,sizeof(char),RECBUF,dfp);
       form_matrix = field_num(buff,5);
       fseek(dfp,savep,0);    
       fseek(pfp,(ptr-1)*RECBUF,0);    
       data = (double *)malloc(13*sizeof(double));
       if (data == NULL) {
          Error(routine,"Allocation Failed");
          return(1);
       }
       if(perf_pro(data,13) > 0){ 
           free(data);
           Error(routine,"***Error occured reading parameter data***");
           return(1);
       }

       switch(form_matrix){
          case 0:
             RotMat[0][0] = data[1];
             RotMat[0][1] = data[2];
             RotMat[0][2] = data[3];
             RotMat[1][0] = data[5];
             RotMat[1][1] = data[6];
             RotMat[1][2] = data[7];
             RotMat[2][0] = data[9];
             RotMat[2][1] = data[10];
             RotMat[2][2] = data[11];

             Trans[0] = data[4];
             Trans[1] = data[8];
             Trans[2] = data[12];

             break;
          default:
             RotMat[0][0] = 1.;
             RotMat[0][1] = 0.;
             RotMat[0][2] = 0.;
             RotMat[1][0] = 0.;
             RotMat[1][1] = 1.;
             RotMat[1][2] = 0.;
             RotMat[2][0] = 0.;
             RotMat[2][1] = 0.;
             RotMat[2][2] = 1.;

             Trans[0] = 0.;
             Trans[1] = 0.;
             Trans[2] = 0.;
             
             Error(routine,"***Matrix form not recognized");
             free(data);
             return(1);
       }

       free(data);
    }
    else{

       RotMat[0][0] = 1.;
       RotMat[0][1] = 0.;
       RotMat[0][2] = 0.;
       RotMat[1][0] = 0.;
       RotMat[1][1] = 1.;
       RotMat[1][2] = 0.;
       RotMat[2][0] = 0.;
       RotMat[2][1] = 0.;
       RotMat[2][2] = 1.;

       Trans[0] = Trans[1] = Trans[2] = 0.;
    }

    return(0);
}

void transform_point_cps( Point *P )
{
   Point p;
   double r[3][3], t[3];

   if(P == NULL){
     Error("transform_point_cps","Point ptr is NULL");
     return;
   }

   if (get_matrix(r,t)) return;

   p.x = P->x;
   p.y = P->y;
   p.z = P->z;

   P->x = p.x * r[0][0] + p.y * r[0][1]
        + p.z * r[0][2] + t[0];

   P->y = p.x * r[1][0] + p.y * r[1][1]
        + p.z * r[1][2] + t[1];

   P->z = p.x * r[2][0] + p.y * r[2][1]
        + p.z * r[2][2] + t[2];

}

void transform_curve_cps( NurbCurv *curv )
{
   int i;
   Hpoint *cps,p;
   double r[3][3], t[3];
   char *routine = "transform_curve_cps";

   if (curv == NULL){
       Error(routine,"Curve ptr is NULL");
       return;
   }

   if (get_matrix(r,t)) return;

   cps = curv->control_point;

   for (i = 0; i <= curv->cp_res; i++)
   {
      p.w = cps[i].w;
      p.x = cps[i].x;
      p.y = cps[i].y;
      p.z = cps[i].z;

      cps[i].x = p.x * r[0][0] + p.y * r[0][1]
               + p.z * r[0][2] + t[0];

      cps[i].y = p.x * r[1][0] + p.y * r[1][1]
               + p.z * r[1][2] + t[1];

      cps[i].z = p.x * r[2][0] + p.y * r[2][1]
               + p.z * r[2][2] + t[2];

      cps[i].w = p.w;

   }

}

void transform_surface_cps( NurbSurf *surf )
{
   int i,j;
   Hpoint **cps,p;
   double r[3][3], t[3];
   char *routine = "transform_surface_cps";

   if (surf == NULL){
       Error(routine,"Curve ptr is NULL");
       return;
   }

   if (get_matrix(r,t)) return;

   cps = surf->control_point;
   for (i = 0; i <= surf->cp_res.k; i++)
   {
      for (j = 0; j <= surf->cp_res.l; j++)
      {
         p.w = cps[i][j].w;
         p.x = cps[i][j].x;
         p.y = cps[i][j].y;
         p.z = cps[i][j].z;
   
         cps[i][j].x = p.x * r[0][0] + p.y * r[0][1]
                     + p.z * r[0][2] + t[0];
   
         cps[i][j].y = p.x * r[1][0] + p.y * r[1][1]
                     + p.z * r[1][2] + t[1];
   
         cps[i][j].z = p.x * r[2][0] + p.y * r[2][1]
                     + p.z * r[2][2] + t[2];
   
         cps[i][j].w = p.w;
      }
   }

}

/*************************************************************/
/* Name    : ins_100()                                       */
/* Author  : Mike Remotigue                                  */
/* Date    : May,  1993                                      */
/* Remark  : in IGES file, data for a circular arc is :      */
/*           Index   Name   Type   Description               */
/*           0       type   real   entity type               */
/*           1       ZT     real   a certain Z plane         */
/*           2       x1     real   Arc center abscissa       */
/*           3       y1     real   Arc center ordinate       */
/*           4       x2     real   Start point abscissa      */
/*           5       y2     real   Start point ordinate      */
/*           6       x3     real   End point abscissa        */
/*           7       y3     real   End point ordinate        */           
/*************************************************************/

NurbCurv *ins_100()
{
    NurbCurv *curv;
    double  *store_b;        /* 7 data for circular arc from IGES */
    long   offset;
    int istat;
    Point start,end,center;
    Hpoint startp, endp;
    double radius;
    char *routine = "ins_100";

    offset = (parameter - 1)*RECBUF;
    fseek(pfp,offset,0);
    store_b = (double *)malloc(sizeof(double)*8);
    if(store_b == NULL)
    {
      Error(routine,"Allocation Failed");
      return(NULL);
    }

    if (perf_pro(store_b,8) > 0)
    {
      Error(routine,"***Error occured reading parameter data***");
      free(store_b);
      return((NurbCurv *)NULL);
    }

    start.x = store_b[4];
    start.y = store_b[5];
    start.z = store_b[1];    /* starting point */

    end.x = store_b[6];
    end.y = store_b[7];
    end.z = store_b[1];    /* ending point */

    center.x = store_b[2];
    center.y = store_b[3];
    center.z = store_b[1];    /* arc center   */

    radius = DISTANCE(center,start);
    if (radius > bound)
    {
      printf(">>>> Radius larger than bound\n");
      Error(routine,"Circular Arc failed");
      free(store_b);
      return NULL;
    }

    if (fabs(radius-DISTANCE(center,end)) > .0001*radius)
    {
      printf(">>>> Radius (%g) is not consistent with %g\n",
             DISTANCE(center,end),radius);
      Error(routine,"Circular Arc failed");
      free(store_b);
      return NULL;
    }

    curv = ArcFromCenterAndEndPoints(center,start,end,&istat);
    if (curv == NULL || istat != NO_ERROR){
       printf("start: %f %f %f\n",start.x,start.y,start.z);
       printf("end: %f %f %f\n",end.x,end.y,end.z);
       printf("center: %f %f %f\n",center.x,center.y,center.z);
       Error(routine,"Circular Arc failed");
       free(store_b);
       return NULL;
    }

    Get3DCurveValueAtU(curv,curv->knot[curv->order-1],&startp,0);
    if (DISTANCE(startp,start) > .000001)
    {
       printf("Given: %g %g %g\n",start.x,start.y,start.z);
       printf("Result: %g %g %g\n",startp.x,startp.y,startp.z);
       Error(routine,"Start point does not match");
       free(store_b);
       return NULL;
    }
    Get3DCurveValueAtU(curv,curv->knot[curv->cp_res+1],&endp,0);
    if (DISTANCE(endp,end) > .000001)
    {
       printf("Given: %g %g %g\n",end.x,end.y,end.z);
       printf("Result: %g %g %g\n",endp.x,endp.y,endp.z);
       Error(routine,"End point does not match");
       free(store_b);
       return NULL;
    }

    if (trans_mtr) transform_curve_cps(curv);

    if (Color != 0) ins_314();

    free(store_b);

    return(curv);
}

/*************************************************************/
/* Name    : ins_102()                                       */
/* Author  : Mike Remotigue                                  */
/* Date    : May,  1993                                      */
/* Remark  : in IGES file, data for a composite curve :      */
/*           Index   Name   Type   Description               */
/*           0       type   real   entity type               */
/*           1       N      int    Number of entities        */
/*           2       DE1    ptr    Pointer to DE 1st entity  */
/*           .       .                                       */
/*           .       .                                       */
/*           .       .                                       */
/*           1+N     DEN    ptr    Pointer to DE last entity */
/*************************************************************/

NurbCurv *ins_102()
{
    NurbCurv *compcurv = NULL, *nc = NULL;
    NurbCurv **curv = NULL;
    double  *store_b;        /* 7 data for circular arc from IGES */
    long   offset;
    int N,i,istat;
    DirEntry_t *DE;
    char *routine = "ins_102";

    offset = (parameter - 1)*RECBUF;
    fseek(pfp,offset,0);
    store_b = (double *)malloc(2*sizeof(double));
    if(store_b == NULL) {
      Error(routine,"Allocation Failed");
      return NULL;
    }
    if (perf_pro(store_b,2) > 0){
        Error(routine,"***Error occured reading parameter data***");
        free(store_b);
        return((NurbCurv *)NULL);
    }

    N = (int)store_b[1];

    free(store_b);

    fseek(pfp,offset,0);
    store_b = (double *)malloc((N+2)*sizeof(double));
    if(store_b == NULL) {
      Error(routine,"Allocation Failed");
      return NULL;
    }
    if (perf_pro(store_b,N+2) > 0){
        Error(routine,"***Error occured reading parameter data***");
        free(store_b);
        return NULL;
    }

    if (N > 1)
    {
       curv = (NurbCurv **)malloc(N*sizeof(NurbCurv *));
       if (curv == NULL) {
          Error(routine,"Allocation Failed");
          free(store_b);
          return NULL;
       }

       for(i=0;i<N;i++){
          DE = find_entry((long)store_b[i+2],1);
          if (DE == NULL){
             Error(routine,"Entry not found");
             free(curv);
             free(store_b);
             return NULL;
          }
          DE->process = 1;
          curv[i] = (NurbCurv *)DE->ndata;
       }

       compcurv = construct_comp_nurb(curv,N,&istat);
       if (compcurv == NULL || istat != NO_ERROR){
          Error(routine,"Composite Curve failed");
          free(curv);
          free(store_b);
          return NULL;
       }
    }
    else
    {
       DE = find_entry((long)store_b[2],1);
       if (DE == NULL){
          Error(routine,"Entry not found");
          free(store_b);
          return NULL;
       }
       DE->process = 1;
       nc = (NurbCurv *)DE->ndata;
       compcurv = Copy_curve(nc);
    }

    if (trans_mtr) transform_curve_cps(compcurv);

    if (Color != 0) ins_314();

    free(store_b);
    if (curv != NULL) free(curv);

    return(compcurv);
}

/*******************************************************************/
/* Function : conic_type()                                         */
/*******************************************************************/
int conic_type(double *store_b)
{
    double A,B,C,D,E,F;
    double q1,q2,q3;

    A = store_b[1];
    B = store_b[2];
    C = store_b[3];
    D = store_b[4];
    E = store_b[5];
    F = store_b[6];

    q1 = A*(C*F-E*E/4)-B*(B*F/2-D*E/4)/2+D*(B*E/4-D*C/2)/2;
    q2 = A*C-B*B/4;
    q3 = A+C;

    if (q2 > 0. && q1*q3 < 0.)
       return 0;
    else if (q2 < 0. && q1 != 0)
       return 1;
    else if (q2 == 0. && q1 != 0)
       return 2;

    return -1;
}

/**************************************************************/
/*                                                            */
/* Program : ins_104, process for the conic arc               */
/* Author  : Robert Yu, modified by Yi Hong for ngp           */
/* Date    : Modified on July 7,1992                          */
/* Remark    : data read from IGES                            */
/*             Index   Name   Type   Description              */
/*               0     type   real   entity type              */
/*               1       A    real   Conic Coefficient        */
/*               2       B    real   Conic Coefficient        */
/*               3       C    real   Conic Coefficient        */
/*               4       D    real   Conic Coefficient        */
/*               5       E    real   Conic Coefficient        */
/*               6       F    real   Conic Coefficient        */
/*               7      ZT    real   a certain Z plane        */
/*               8      x1    real   Start point abscissa     */
/*               9      y1    real   Start point ordinate     */
/*              10      x2    real   End point abscissa       */
/*              11      y2    real   End point ordinate       */           
/**************************************************************/
 
NurbCurv *ins_104()
{
    NurbCurv *curv = NULL;
    int type, istat;
    double  *store_b = NULL; 
    long offset;
    Point sp,ep,center,vn;
    Point xaxis = {1,0,0};
    Point nxaxis = {-1,0,0};
    double sang,eang;
    double a,b;
    char *routine = "ins_104";
    char message[200];
    double rad;

    offset = (parameter-1)*RECBUF;
    fseek(pfp,offset,0);

    store_b = (double *) malloc(12*sizeof(double));
    if (store_b == NULL) {
       Error(routine,"Allocation FAiled");
       return ((NurbCurv *)NULL);
    }

    if (perf_pro(store_b,12) > 0){
        Error(routine,"***Error occured reading parameter data***");
        free(store_b);
        return((NurbCurv *)NULL);
    }

    type = conic_type(store_b);

    sp.x = store_b[8];
    sp.y = store_b[9];
    sp.z = 0.;

    ep.x = store_b[10];
    ep.y = store_b[11];
    ep.z = 0.;

    center.x = 0.;
    center.y = 0.;
    center.z = store_b[7];

    if (same_Point(sp,ep,.00005))
    {
       switch(type)
       {
         case 0:
            a = sqrt(-store_b[6]/store_b[1]);
            b = sqrt(-store_b[6]/store_b[3]);
            curv = FullEllipse(a,b,center,PLANE_XY,0.,&istat);
            break;
         default:
            Error(routine,"Conic type unknown");
            free(store_b);
            return NULL;
       }
    }
    else
    {
       switch(type)
       {
         case 0:
            a = sqrt(-store_b[6]/store_b[1]);
            b = sqrt(-store_b[6]/store_b[3]);
            rad = sp.x/a;
            if (fabs(fabs(rad)-1.) < 1.e-6) rad = rad/fabs(rad);
            sang = RAD_TO_DEG(acos(rad));
            if (sp.y < 0.) sang = 360.-sang;
            rad = ep.x/a;
            if (fabs(fabs(rad)-1.) < 1.e-6) rad = rad/fabs(rad);
            eang = RAD_TO_DEG(acos(rad));
            if (ep.y < 0.) eang = 360.-eang;
            curv = EllipticArcFromAngles(a,b,center,0.,sang,eang,
                                         PLANE_XY,&istat);
            if (curv == NULL)
            {
              sprintf(message,"Failed creating conic arc:\n\tStart Point: %g %g End Point: %g %g\n\tAngle: Start=%g End=%g\n\tA=%g B=%g Center: %g %g %g",sp.x,sp.y,ep.x,ep.y,sang,eang,a,b,center.x,center.y,center.z);
              Error(routine,message);
              return NULL;
            }
            break;
         case 1:
         case 2:
         default:
            Error(routine,"Conic type unknown");
            free(store_b);
            return NULL;
       }
    }

    free(store_b);

    if (trans_mtr) transform_curve_cps(curv);

    if (Color != 0) ins_314();

    return(curv); 
}


/* program : ins_106 ----- do the process for the copious data */
/* ----------------------------------------------------------- */
/* for this program,we just process the form # equal 1,2 and 3 */
/* according to NASA-IGES Specification Draft                  */

void ins_106(menuCalldata *info,int *pid,int *lid,void **VP)
{
    Point *P;
    Point *start,*end;
    Vector *v;
    NurbCurv *curv;
    double *data = NULL;
    long   offset, maxdata;
    int    ip,np,n,j=0,nn;
    int    istat;
    char *routine = "ins_106";

/* first ,get the information of ip and np from the parameter file */
    offset = (parameter - 1)*RECBUF;
    fseek(pfp,offset,0);
    data = (double *) malloc(sizeof(double)*3);
    if (data == NULL)
    {
      Error(routine,"Allocation Failed");
      return;
    }

/* we assume the entype,ip,np are located in the first line */

    if(perf_pro(data,3) > 0)
    {
      Error(routine,"***Error occured reading parameter data***");
      free(data);
      return;
    }

/* after the above process,we should get the entype,ip,np */

   ip = (int) data[1] ; /* and entype = data[0] */
   np = (int) data[2] ;
   free(data);  /* release the memory */

   switch (ip) 
   {
     case 1  :
              maxdata = 4+2*np;
              break ;
     case 2  : 
              maxdata = 3+3*np;
              break ;
     case 3  : 
              maxdata = 3+6*np;
    	      break ;
     default :
              return;
   }  

   data  = (double *) malloc((unsigned int)(sizeof(double)*maxdata));
   if (data == NULL)
   {
     Error(routine,"Allocation Failed");
     return;
   }

/* since we have allocated proper memory for the array, we can now 
   access the parameter file and store the information to data   */

   fseek(pfp,offset,0);
   if(perf_pro(data,(int)maxdata) > 0)
   { 
     Error(routine,"***Error occured reading parameter data***");
     free(data);
     return;
   }

   switch (form) 
   {
     case  1:
     case 11:
     case 63: /* 2D polyline */
              P = (Point *)malloc(np*sizeof(Point));
              if(P == NULL)
              {
                Error(routine,"Allocation Failed");
                break;
              }
              for ( nn=n=0,j=4;n<np;j+=2,n++)
              {
                P[nn].x = data[j];
                P[nn].y = data[j+1];
                P[nn].z = data[3];
                if (nn > 1 && P[nn].x == P[nn-1].x && P[nn].y == P[nn-1].y &&
                              P[nn].z == P[nn-1].z) continue;
                nn++;
              }
              curv = LinearCurveInterpolation(nn-1,P,OPEN,UNIFORM,n+1,&istat);
              free(P);
              if (curv == NULL || istat != NO_ERROR)
              {
                 Error(routine,"Error in interpolation of curve points");
                 break;
              }
              if (trans_mtr) transform_curve_cps(curv);

              VP[0] = (void *)curv;
              *lid=1;
              break ;
     case  2:
     case 12:  /* 3D polyline */
              P  = (Point *)malloc(np*sizeof(Point));
              if (P == NULL)
              {
                 Error(routine,"Allocation Failed");
                 break;
              }
              for ( nn=n=0,j=3;n<np;j+=3,n++)
              { 
                  P[nn].x = data[j];
                  P[nn].y = data[j+1];
                  P[nn].z = data[j+2];
                  if (nn > 1 && P[nn].x == P[nn-1].x && P[nn].y == P[nn-1].y &&
                                P[nn].z == P[nn-1].z) continue;
                  nn++;
              }
              curv = LinearCurveInterpolation(nn-1,P,OPEN,UNIFORM,n+1,&istat);
              
              free(P);
              if (curv == NULL || istat != NO_ERROR){
                 Error(routine,"Error in interpolation of curve points");
                 break;
              }
              if (trans_mtr) transform_curve_cps(curv);
              if (Color != 0) ins_314();

              VP[0] = (void *)curv;
              *lid=1;
              break ;
     case  3:
     case 13: /* 3D polyline with vectors */
              if (np > 1)
              {
                P = (Point *)malloc(np*sizeof(Point));
                if (P == NULL)
                {
                   Error(routine,"Allocation Failed");
                   break;
                }
                for (nn=n=0,j=3;n<np;j+=6,n++)
                {
                    P[nn].x = data[j];
                    P[nn].y = data[j+1];
                    P[nn].z = data[j+2];
                    if (nn > 1 && P[nn].x == P[nn-1].x && 
                                  P[nn].y == P[nn-1].y &&
                                  P[nn].z == P[nn-1].z) continue;
                    nn++;
                }
                curv = LinearCurveInterpolation(nn-1,P,OPEN,UNIFORM,n+1,&istat);
                free(P);
                if (curv == NULL || istat != NO_ERROR)
                {
                   Error(routine,"Error in interpolation of curve points");
                   break;
                }
                if (trans_mtr) transform_curve_cps(curv);
                if (Color != 0) ins_314();

                VP[0] = (void *)curv;
                *lid=1;
              }
              else if (process == 0)
              {
                start = (Point *)malloc(sizeof(Point));
                end = (Point *)malloc(sizeof(Point));
                if (start != NULL && end != NULL){
                   start->x = data[3];
                   start->y = data[4];
                   start->z = data[5];

                   end->x = start->x+data[6];
                   end->y = start->y+data[7];
                   end->z = start->z+data[8];

                   v = CreateVector(info,start,end);
                   if (v == NULL)
                   {
                      Error(routine,"Memory allocation failed");
                      free(start); free(end);
                      break;
                   }
                   if (visible != 0) v->draw_props = INVISIBLE;

                   v->vx = data[j+3];
                   v->vy = data[j+4];
                   v->vz = data[j+5];
 
                   v->mag = sqrt(v->vx*v->vx+v->vy*v->vy+v->vz*v->vz);
  
                   v->vx /= v->mag;
                   v->vy /= v->mag;
                   v->vz /= v->mag;
                }
              }
              break ;
   }  

   free(data);
   return;
}

NurbSurf *ins_108()
{
    double  *store_b, A, B, C, D;
    int     curv;
    long   offset;
    NurbSurf *surf = NULL;
    char *routine = "ins_108";

    /*Get values of starting and ending points by using */
    /*the fseek function with the offset parameter   */
    offset = (parameter - 1)*RECBUF;
    fseek(pfp,offset,0);
    store_b = (double *)malloc(sizeof(double)*10);
    if (store_b == NULL) {
       Error(routine,"Allocation Failed");
       return(NULL);
    }
    if(perf_pro(store_b,10) > 0){
        Error(routine,"***Error occured reading parameter data***");
        free(store_b);
        return(NULL);
    }

    A = store_b[1];
    B = store_b[2];
    C = store_b[3];
    D = store_b[4];
    curv = (int)store_b[5];

    free(store_b);

    if (trans_mtr) transform_surface_cps(surf);
    if (Color != 0) ins_314();
    return(surf);
}

NurbCurv *ins_110(int line_id)
{
    double  *store_b; long   offset;
    NurbCurv *curv; int istat;
    char *routine = "ins_110";
    Point start,end;

    /*Get values of starting and ending points by using */
    /*the fseek function with the offset parameter   */
    offset = (parameter - 1)*RECBUF;
    fseek(pfp,offset,0);
    store_b = (double *)malloc(sizeof(double)*7);
    if (store_b == NULL) {
       Error(routine,"Allocation Failed");
       return((NurbCurv *)NULL);
    }
    if(perf_pro(store_b,7) > 0){
        Error(routine,"***Error occured reading parameter data***");
        free(store_b);
        return((NurbCurv *)NULL);
    }

    start.x = store_b[1];
    start.y = store_b[2];
    start.z = store_b[3];
    end.x = store_b[4];
    end.y = store_b[5];
    end.z = store_b[6];

    free(store_b);

    curv = BJ_NURB_line(start,end,0,&istat);
    if (curv == NULL || istat != NO_ERROR){
       Error(routine,"Error in BJ_NURB_line");
       return NULL;
    }
    if (trans_mtr) transform_curve_cps(curv);
    if (Color != 0) ins_314();
    return(curv);
}

void evaluate_parametric_curve(double u,Hpoint *p,double *ss,int ns,int deriv)
{
  int      i;
  double   ax,bx,cx,dx,ay,by,cy,dy,az,bz,cz,dz,s;

  i = 0;
  while (!(u >= ss[5+i] && u < ss[6+i] && i < ns))
    i++;

  ax = ss[ 6+ns+i*12];
  bx = ss[ 7+ns+i*12];
  cx = ss[ 8+ns+i*12];
  dx = ss[ 9+ns+i*12];

  ay = ss[10+ns+i*12];
  by = ss[11+ns+i*12];
  cy = ss[12+ns+i*12];
  dy = ss[13+ns+i*12];

  az = ss[14+ns+i*12];
  bz = ss[15+ns+i*12];
  cz = ss[16+ns+i*12];
  dz = ss[17+ns+i*12];

  s = u - ss[5+i];

  if (deriv == 0)
  {
     (*p).x = ax+bx*s+cx*s*s+dx*s*s*s;
     (*p).y = ay+by*s+cy*s*s+dy*s*s*s;
     (*p).z = az+bz*s+cz*s*s+dz*s*s*s;
  }
  else if (deriv == 1)
  {
     (*p).x = bx+2.*cx*s+3.*dx*s*s;
     (*p).y = by+2.*cy*s+3.*dy*s*s;
     (*p).z = bz+2.*cz*s+3.*dz*s*s;
  }
  else if (deriv == 2)
  {
     (*p).x = 2.*cx+6.*dx*s;
     (*p).y = 2.*cy+6.*dy*s;
     (*p).z = 2.*cz+6.*dz*s;
  }
}

void get_parametric_curve_coef(double u,Hpoint *A,double *ss,int ns)
{
  int i,ii;
  double s[4];

  ii = 0;
  while (!(u >= ss[5+ii] && u < ss[6+ii] && ii < ns))
    ii++;

  s[0] = 1.;
  s[1] = ss[5+ii+1]-ss[5+ii];
  s[2] = s[1]*s[1];
  s[3] = s[2]*s[1];

  for (i=0;i<4;i++)
  {
    A[i].x = ss[ 6+i+ns+ii*12]*s[i];
    A[i].y = ss[10+i+ns+ii*12]*s[i];
    A[i].z = ss[14+i+ns+ii*12]*s[i];
  }
}

NurbCurv *get_nurb_curv_from_parametric(double *ss,int m)
      
{
  int       M,K;
  int       i,j,I;
  double    u;
  NurbCurv  *nc;
  Hpoint    *cp;
  Hpoint    **A;
  double    umin,umax;
  double    error,max_error=0.;
  Hpoint    pn,pc;
  int       npts = 51;
  char      *routine = "get_nurb_curv_from_parametric";

  umin = ss[5];
  umax = ss[5+m];

  M = m*3;
  K = 4;

  nc = AllocateNurbCurv(0,K,M);
  if (nc == NULL)
  {
     Error(routine,"Failed to allocate NURBS Curve");
     return NULL;
  }

  A = (Hpoint **)malloc(m*sizeof(Hpoint *));
  for(i=0;i<m;i++)
     A[i] = (Hpoint *)malloc(4*sizeof(Hpoint));

  for (j=0;j<m;j++){
     u = ss[5+j];
     get_parametric_curve_coef(u,A[j],ss,m);
  }

  cp = nc->control_point;

  for(I=i=0;i<m;i++,I+=3)
  {
    cp[I].x = A[i][0].x;
    cp[I].y = A[i][0].y;
    cp[I].z = A[i][0].z;
    cp[I].w = 1.;
  }
  cp[M].x = A[m-1][0].x+A[m-1][1].x+A[m-1][2].x+A[m-1][3].x;
  cp[M].y = A[m-1][0].y+A[m-1][1].y+A[m-1][2].y+A[m-1][3].y;
  cp[M].z = A[m-1][0].z+A[m-1][1].z+A[m-1][2].z+A[m-1][3].z;
  cp[M].w = 1.;

  for(I=i=0;i<m;i++,I+=3)
  {
    cp[I+1].x = A[i][0].x+A[i][1].x/3.;
    cp[I+1].y = A[i][0].y+A[i][1].y/3.;
    cp[I+1].z = A[i][0].z+A[i][1].z/3.;
    cp[I+1].w = 1.;

    cp[I+2].x = A[i][0].x+2.*A[i][1].x/3.+A[i][2].x/3.;
    cp[I+2].y = A[i][0].y+2.*A[i][1].y/3.+A[i][2].y/3.;
    cp[I+2].z = A[i][0].z+2.*A[i][1].z/3.+A[i][2].z/3.;
    cp[I+2].w = 1.;
  }

  for(I=1,i=0;i<=m;i++)
    for(j=1;j<=3;j++,I++)
      nc->knot[I] = ss[5+i];
  nc->knot[0] = nc->knot[1];
  nc->knot[I] = nc->knot[I-1];

  nc->u_min = umin;
  nc->u_max = umax;

  if (PrintOut == 1)
  {
    for(i=1;i<npts;i++)
    {
       u = umin+(double)i*(umax-umin)/(double)npts;
       evaluate_parametric_curve(u,&pc,ss,m,0);
       Get3DCurveValueAtU(nc,u,&pn,0);
       error = DISTANCE(pc,pn);
       if (error > max_error) max_error = error;
    }
    printf(" max_error = %g\n",max_error);
  }

  for(i=0;i<m;i++)
     free(A[i]);
  free(A);

  return nc;
}

NurbCurv *ins_112()
{
    NurbCurv *curv;
    double  *store_b;
    long   offset;
    int    n;
    char *routine = "ins_112";

/* first ,get the information of n from the parameter file */
/* we do this to ensure to have proper dimension for store_b */
    offset = ((parameter - 1)*RECBUF);
    fseek(pfp,offset,0);

    store_b=(double *)malloc(5*sizeof(double));
    if (store_b == NULL) {
       Error(routine,"Allocation Failed");
       return NULL;
    }

    if(perf_pro(store_b,5) > 0){
       Error(routine,"***Error occured reading parameter data***");
       free(store_b);
       return (NULL);
    }

/* after the above process,we should get the entype,ctype,h,
   hdim and n  */

    n = (int)store_b[4] ;
    free(store_b) ;        /* release the memory */

    fseek(pfp,offset,0);

/* since we know the n , we are able to allocate a proper memory */

    store_b = (double *)malloc((5+13*n)*sizeof(double));/*p87 of IGES menu*/
    if(store_b == NULL){
      Error(routine,"Allocation Failed");
      return NULL;
    }

    if(perf_pro(store_b,5+13*n) > 0){
        Error(routine,"***Error occured reading parameter data***");
        free(store_b);
        return (NULL);
    }

    if (PrintOut == 1) printf(" %d segments\n",n);

    curv = get_nurb_curv_from_parametric(store_b,n);
    if (curv == NULL) {
       Error(routine,"Failed calculating NURBS Curve");
       free(store_b);
       return NULL;
    }

    if (trans_mtr) transform_curve_cps(curv);
    if (Color != 0) ins_314();

    free(store_b);
    return (curv);
}

void evaluate_parametric_surface(double u,double v,Hpoint *p,double *ss,
                                 int m,int n,int uderiv,int vderiv)
{
  int       k,l,ii,jj,nn;
  double    x[4][4],y[4][4],z[4][4];
  double    s[4],t[4];

  jj = 0;
  while (!(v >= ss[6+m+jj] && v < ss[7+m+jj]) && jj < n)
    jj++;

  ii = 0;
  while (!(u >= ss[5+ii] && u < ss[6+ii]) && ii < m )
    ii++;

  for (nn=0,l=0;l<4;l++)
    for (k=0;k<4;k++){
      x[k][l] = ss[ 7+nn+m+n+48*(ii*(n+1)+jj)];
      y[k][l] = ss[23+nn+m+n+48*(ii*(n+1)+jj)];
      z[k][l] = ss[39+nn+m+n+48*(ii*(n+1)+jj)];
      nn++;
    }

  if (uderiv == 0 && vderiv == 0)
  {
     s[0] = 1.;
     s[1] = u-ss[5+ii];
     s[2] = s[1]*s[1];
     s[3] = s[2]*s[1];

     t[0] = 1.;
     t[1] = v-ss[6+m+jj];
     t[2] = t[1]*t[1];
     t[3] = t[2]*t[1];
  }
  else if (uderiv == 1 && vderiv == 0)
  {
     s[0] = 0.;
     s[1] = 1.;
     s[2] = 2.*(u-ss[5+ii]);
     s[3] = 3.*(u-ss[5+ii])*(u-ss[5+ii]);

     t[0] = 1.;
     t[1] = v-ss[6+m+jj];
     t[2] = t[1]*t[1];
     t[3] = t[2]*t[1];
  }
  else if (uderiv == 0 && vderiv == 1)
  {
     s[0] = 1.;
     s[1] = u-ss[5+ii];
     s[2] = s[1]*s[1];
     s[3] = s[2]*s[1];

     t[0] = 0.;
     t[1] = 1.;
     t[2] = 2.*(v-ss[6+m+jj]);
     t[3] = 3.*(v-ss[6+m+jj])*(v-ss[6+m+jj]);
  }
  else if (uderiv == 1 && vderiv == 1)
  {
     s[0] = 0.;
     s[1] = 1.;
     s[2] = 2.*(u-ss[5+ii]);
     s[3] = 3.*(u-ss[5+ii])*(u-ss[5+ii]);

     t[0] = 0.;
     t[1] = 1.;
     t[2] = 2.*(v-ss[6+m+jj]);
     t[3] = 3.*(v-ss[6+m+jj])*(v-ss[6+m+jj]);
  }
  else if (uderiv == 2 && vderiv == 0)
  {
     s[0] = 0.;
     s[1] = 0.;
     s[2] = 2.;
     s[3] = 6.*(u-ss[5+ii]);

     t[0] = 1.;
     t[1] = v-ss[6+m+jj];
     t[2] = t[1]*t[1];
     t[3] = t[2]*t[1];
  }
  else if (uderiv == 0 && vderiv == 2)
  {
     s[0] = 1.;
     s[1] = u-ss[5+ii];
     s[2] = s[1]*s[1];
     s[3] = s[2]*s[1];

     t[0] = 0.;
     t[1] = 0.;
     t[2] = 2.;
     t[3] = 6.*(v-ss[6+m+jj]);
  }

  (*p).x = (*p).y = (*p).z = 0.;
  for(l=0;l<4;l++)
    for(k=0;k<4;k++){
      (*p).x += x[k][l]*s[k]*t[l];
      (*p).y += y[k][l]*s[k]*t[l];
      (*p).z += z[k][l]*s[k]*t[l];
    }
}

void get_parametric_surface_coef(double u,double v,Hpoint *A,double *ss,
                                 int m,int n)
{
  int k,l,ii,jj,nn;
  double s[4],t[4];

  jj = 0;
  while (!(v >= ss[6+m+jj] && v < ss[7+m+jj]) && jj < n)
    jj++;

  ii = 0;
  while (!(u >= ss[5+ii] && u < ss[6+ii]) && ii < m )
    ii++;

  s[0] = 1.;
  s[1] = ss[6+ii]-ss[5+ii];
  s[2] = s[1]*s[1];
  s[3] = s[2]*s[1];

  t[0] = 1.;
  t[1] = ss[7+m+jj]-ss[6+m+jj];
  t[2] = t[1]*t[1];
  t[3] = t[2]*t[1];

  for (nn=0,l=0;l<4;l++)
    for (k=0;k<4;k++){
      A[k+l*4].x = ss[ 7+nn+m+n+48*(ii*(n+1)+jj)]*s[k]*t[l];
      A[k+l*4].y = ss[23+nn+m+n+48*(ii*(n+1)+jj)]*s[k]*t[l];
      A[k+l*4].z = ss[39+nn+m+n+48*(ii*(n+1)+jj)]*s[k]*t[l];
      nn++;
    }
}

NurbSurf *get_nurb_surf_from_parametric(double *ss,int m,int n)
{
  int       M,N,K,L;
  int       i,j,nn,I,J;
  double    u,v;
  NurbSurf  *ns;
  Hpoint    **cp;
  Hpoint    ***A;
  double    umin,umax,vmin,vmax;
  double    error, max_error = 0.;
  Hpoint    pn,ps;
  int       npts = 31;
  char      *routine = "get_nurb_surf_from_parametric";

  M = m*3;
  N = n*3;
  K = 4;
  L = 4;

  umin = ss[5];
  umax = ss[5+m];
  vmin = ss[6+m];
  vmax = ss[6+m+n];

  ns = AllocateNurbSurf(0,K,L,M,N);
  if (ns == NULL)
  {
     Error(routine,"Failed to allocate NURBS Surface");
     return NULL;
  }

  A = (Hpoint ***)malloc(m*sizeof(Hpoint **));
  for (i=0;i<m;i++)
     A[i] = (Hpoint **)malloc(n*sizeof(Hpoint *));
  for (j=0;j<n;j++)
     for (i=0;i<m;i++)
        A[i][j] = (Hpoint *)malloc(16*sizeof(Hpoint));

  for (i=0;i<m;i++)
  {
    u = ss[5+i];
    for (j=0;j<n;j++)
    {
      v = ss[6+m+j];
      get_parametric_surface_coef(u,v,A[i][j],ss,m,n);
    }
  }

  cp = ns->control_point;

  for(I=i=0;i<m;i++,I+=3)
  {
    for(J=j=0;j<n;j++,J+=3)
    {
      cp[I][J].x = A[i][j][0].x;
      cp[I][J].y = A[i][j][0].y;
      cp[I][J].z = A[i][j][0].z;
      cp[I][J].w = 1.;

      cp[I+1][J].x = A[i][j][0].x+A[i][j][1].x/3.;
      cp[I+1][J].y = A[i][j][0].y+A[i][j][1].y/3.;
      cp[I+1][J].z = A[i][j][0].z+A[i][j][1].z/3.;
      cp[I+1][J].w = 1.;

      cp[I+2][J].x = A[i][j][0].x+2.*A[i][j][1].x/3.+A[i][j][2].x/3.;
      cp[I+2][J].y = A[i][j][0].y+2.*A[i][j][1].y/3.+A[i][j][2].y/3.;
      cp[I+2][J].z = A[i][j][0].z+2.*A[i][j][1].z/3.+A[i][j][2].z/3.;
      cp[I+2][J].w = 1.;

      cp[I+3][J].x = A[i][j][0].x+A[i][j][1].x+A[i][j][2].x+A[i][j][3].x;
      cp[I+3][J].y = A[i][j][0].y+A[i][j][1].y+A[i][j][2].y+A[i][j][3].y;
      cp[I+3][J].z = A[i][j][0].z+A[i][j][1].z+A[i][j][2].z+A[i][j][3].z;
      cp[I+3][J].w = 1.;

      cp[I+3][J+1].x = A[i][j][0].x+A[i][j][1].x+A[i][j][2].x+A[i][j][3].x
                     + (A[i][j][4].x+A[i][j][5].x+A[i][j][6].x+A[i][j][7].x)/3.;
      cp[I+3][J+1].y = A[i][j][0].y+A[i][j][1].y+A[i][j][2].y+A[i][j][3].y
                     + (A[i][j][4].y+A[i][j][5].y+A[i][j][6].y+A[i][j][7].y)/3.;
      cp[I+3][J+1].z = A[i][j][0].z+A[i][j][1].z+A[i][j][2].z+A[i][j][3].z
                     + (A[i][j][4].z+A[i][j][5].z+A[i][j][6].z+A[i][j][7].z)/3.;
      cp[I+3][J+1].w = 1.;

      cp[I+3][J+2].x = A[i][j][0].x+A[i][j][1].x+A[i][j][2].x+A[i][j][3].x+
                    2.*(A[i][j][4].x+A[i][j][5].x+A[i][j][6].x+A[i][j][7].x)/3.+
                     (A[i][j][8].x+A[i][j][9].x+A[i][j][10].x+A[i][j][11].x)/3.;
      cp[I+3][J+2].y = A[i][j][0].y+A[i][j][1].y+A[i][j][2].y+A[i][j][3].y+
                    2.*(A[i][j][4].y+A[i][j][5].y+A[i][j][6].y+A[i][j][7].y)/3.+
                     (A[i][j][8].y+A[i][j][9].y+A[i][j][10].y+A[i][j][11].y)/3.;
      cp[I+3][J+2].z = A[i][j][0].z+A[i][j][1].z+A[i][j][2].z+A[i][j][3].z+
                    2.*(A[i][j][4].z+A[i][j][5].z+A[i][j][6].z+A[i][j][7].z)/3.+
                     (A[i][j][8].z+A[i][j][9].z+A[i][j][10].z+A[i][j][11].z)/3.;
      cp[I+3][J+2].w = 1.;

      cp[I+3][J+3].x = A[i][j][ 0].x+A[i][j][ 1].x+A[i][j][ 2].x+A[i][j][ 3].x
                     + A[i][j][ 4].x+A[i][j][ 5].x+A[i][j][ 6].x+A[i][j][ 7].x
                     + A[i][j][ 8].x+A[i][j][ 9].x+A[i][j][10].x+A[i][j][11].x
                     + A[i][j][12].x+A[i][j][13].x+A[i][j][14].x+A[i][j][15].x;
      cp[I+3][J+3].y = A[i][j][ 0].y+A[i][j][ 1].y+A[i][j][ 2].y+A[i][j][ 3].y
                     + A[i][j][ 4].y+A[i][j][ 5].y+A[i][j][ 6].y+A[i][j][ 7].y
                     + A[i][j][ 8].y+A[i][j][ 9].y+A[i][j][10].y+A[i][j][11].y
                     + A[i][j][12].y+A[i][j][13].y+A[i][j][14].y+A[i][j][15].y;
      cp[I+3][J+3].z = A[i][j][ 0].z+A[i][j][ 1].z+A[i][j][ 2].z+A[i][j][ 3].z
                     + A[i][j][ 4].z+A[i][j][ 5].z+A[i][j][ 6].z+A[i][j][ 7].z
                     + A[i][j][ 8].z+A[i][j][ 9].z+A[i][j][10].z+A[i][j][11].z
                     + A[i][j][12].z+A[i][j][13].z+A[i][j][14].z+A[i][j][15].z;
      cp[I+3][J+3].w = 1.;

      cp[I+2][J+3].x = A[i][j][0].x+A[i][j][4].x+A[i][j][8].x+A[i][j][12].x+
                   2.*(A[i][j][1].x+A[i][j][5].x+A[i][j][9].x+A[i][j][13].x)/3.+
                     (A[i][j][2].x+A[i][j][6].x+A[i][j][10].x+A[i][j][14].x)/3.;
      cp[I+2][J+3].y = A[i][j][0].y+A[i][j][4].y+A[i][j][8].y+A[i][j][12].y+
                   2.*(A[i][j][1].y+A[i][j][5].y+A[i][j][9].y+A[i][j][13].y)/3.+
                     (A[i][j][2].y+A[i][j][6].y+A[i][j][10].y+A[i][j][14].y)/3.;
      cp[I+2][J+3].z = A[i][j][0].z+A[i][j][4].z+A[i][j][8].z+A[i][j][12].z+
                   2.*(A[i][j][1].z+A[i][j][5].z+A[i][j][9].z+A[i][j][13].z)/3.+
                     (A[i][j][2].z+A[i][j][6].z+A[i][j][10].z+A[i][j][14].z)/3.;
      cp[I+2][J+3].w = 1.;

      cp[I+1][J+3].x = A[i][j][0].x+A[i][j][4].x+A[i][j][8].x+A[i][j][12].x
                     +(A[i][j][1].x+A[i][j][5].x+A[i][j][9].x+A[i][j][13].x)/3.;
      cp[I+1][J+3].y = A[i][j][0].y+A[i][j][4].y+A[i][j][8].y+A[i][j][12].y
                     +(A[i][j][1].y+A[i][j][5].y+A[i][j][9].y+A[i][j][13].y)/3.;
      cp[I+1][J+3].z = A[i][j][0].z+A[i][j][4].z+A[i][j][8].z+A[i][j][12].z
                     +(A[i][j][1].z+A[i][j][5].z+A[i][j][9].z+A[i][j][13].z)/3.;
      cp[I+1][J+3].w = 1.;

      cp[I][J+3].x = A[i][j][0].x+A[i][j][4].x+A[i][j][8].x+A[i][j][12].x;
      cp[I][J+3].y = A[i][j][0].y+A[i][j][4].y+A[i][j][8].y+A[i][j][12].y;
      cp[I][J+3].z = A[i][j][0].z+A[i][j][4].z+A[i][j][8].z+A[i][j][12].z;
      cp[I][J+3].w = 1.;

      cp[I][J+2].x = A[i][j][0].x+2.*A[i][j][4].x/3.+A[i][j][8].x/3.;
      cp[I][J+2].y = A[i][j][0].y+2.*A[i][j][4].y/3.+A[i][j][8].y/3.;
      cp[I][J+2].z = A[i][j][0].z+2.*A[i][j][4].z/3.+A[i][j][8].z/3.;
      cp[I][J+2].w = 1.;

      cp[I][J+1].x = A[i][j][0].x+A[i][j][4].x/3.;
      cp[I][J+1].y = A[i][j][0].y+A[i][j][4].y/3.;
      cp[I][J+1].z = A[i][j][0].z+A[i][j][4].z/3.;
      cp[I][J+1].w = 1.;

      cp[I+1][J+1].x = A[i][j][0].x+A[i][j][1].x/3.
                     + A[i][j][4].x/3.+A[i][j][5].x/9.;
      cp[I+1][J+1].y = A[i][j][0].y+A[i][j][1].y/3.
                     + A[i][j][4].y/3.+A[i][j][5].y/9.;
      cp[I+1][J+1].z = A[i][j][0].z+A[i][j][1].z/3.
                     + A[i][j][4].z/3.+A[i][j][5].z/9.;
      cp[I+1][J+1].w = 1.;

      cp[I+2][J+1].x = A[i][j][0].x+2.*A[i][j][1].x/3.+A[i][j][2].x/3.
                     + A[i][j][4].x/3.+2.*A[i][j][5].x/9.+A[i][j][6].x/9.;
      cp[I+2][J+1].y = A[i][j][0].y+2.*A[i][j][1].y/3.+A[i][j][2].y/3.
                     + A[i][j][4].y/3.+2.*A[i][j][5].y/9.+A[i][j][6].y/9.;
      cp[I+2][J+1].z = A[i][j][0].z+2.*A[i][j][1].z/3.+A[i][j][2].z/3.
                     + A[i][j][4].z/3.+2.*A[i][j][5].z/9.+A[i][j][6].z/9.;
      cp[I+2][J+1].w = 1.;

      cp[I+1][J+2].x = A[i][j][0].x+A[i][j][1].x/3.+2.*A[i][j][4].x/3.
                     + 2.*A[i][j][5].x/9.+A[i][j][8].x/3.+A[i][j][9].x/9.;
      cp[I+1][J+2].y = A[i][j][0].y+A[i][j][1].y/3.+2.*A[i][j][4].y/3.
                     + 2.*A[i][j][5].y/9.+A[i][j][8].y/3.+A[i][j][9].y/9.;
      cp[I+1][J+2].z = A[i][j][0].z+A[i][j][1].z/3.+2.*A[i][j][4].z/3.
                     + 2.*A[i][j][5].z/9.+A[i][j][8].z/3.+A[i][j][9].z/9.;
      cp[I+1][J+2].w = 1.;

      cp[I+2][J+2].x = A[i][j][0].x+2.*A[i][j][1].x/3.+A[i][j][2].x/3.
                     + 2.*A[i][j][4].x/3.+4.*A[i][j][5].x/9.+2.*A[i][j][6].x/9.
                     + A[i][j][8].x/3.+2.*A[i][j][9].x/9.+A[i][j][10].x/9.;
      cp[I+2][J+2].y = A[i][j][0].y+2.*A[i][j][1].y/3.+A[i][j][2].y/3.
                     + 2.*A[i][j][4].y/3.+4.*A[i][j][5].y/9.+2.*A[i][j][6].y/9.
                     + A[i][j][8].y/3.+2.*A[i][j][9].y/9.+A[i][j][10].y/9.;
      cp[I+2][J+2].z = A[i][j][0].z+2.*A[i][j][1].z/3.+A[i][j][2].z/3.
                     + 2.*A[i][j][4].z/3.+4.*A[i][j][5].z/9.+2.*A[i][j][6].z/9.
                     + A[i][j][8].z/3.+2.*A[i][j][9].z/9.+A[i][j][10].z/9.;
      cp[I+2][J+2].w = 1.;

    }
  }

  for(I=1,i=0;i<=m;i++)
    for(nn=1;nn<=3;nn++,I++)
      ns->knotU[I] = ss[5+i];
  ns->knotU[0] = ns->knotU[1];
  ns->knotU[I] = ns->knotU[I-1];

  ns->u_min = umin;
  ns->u_max = umax;

  for(J=1,j=0;j<=n;j++)
    for(nn=1;nn<=3;nn++,J++)
      ns->knotV[J] = ss[6+m+j];
  ns->knotV[0] = ns->knotV[1];
  ns->knotV[J] = ns->knotV[J-1];

  ns->v_min = vmin;
  ns->v_max = vmax;

  if (PrintOut == 1)
  {
    for(i=1;i<npts;i++)
    {
       u = ns->u_min+(double)i*(ns->u_max-ns->u_min)/(double)npts;
       for(j=0;j<npts;j++)
       {
          v = ns->v_min+(double)j*(ns->v_max-ns->v_min)/(double)npts;
          evaluate_parametric_surface(u,v,&ps,ss,m,n,0,0);
          Get3DSurfValueAtUV(ns,u,v,&pn,0,0);
          if ((error = DISTANCE(ps,pn)) > max_error) max_error = error;
       }
    }
    printf(" max_error = %g\n",max_error);
  }

  for(j=0;j<n;j++)
     for(i=0;i<m;i++)
        free(A[i][j]);
  for(i=0;i<m;i++)
     free(A[i]);
  free(A);

  return ns;
}

NurbSurf *ins_114()
{
   NurbSurf *surf = NULL;
   double  *store_b;
   long   offset;
   int    m,n,max_data;
   char *routine = "ins_114";
   void get_p_surf(Point **,double *,int,int,int,int,double *,double *,double *,
                   double *);
   
/* first ,get the information of m and n from the parameter file */

   offset = (parameter - 1)*RECBUF;
   fseek(pfp,offset,0);

   store_b = (double *)malloc(5*sizeof(double));
   if (store_b == NULL) {
      Error(routine,"Allocation Failed");
      return NULL;
   }
   if (perf_pro(store_b,5) > 0){
       Error(routine,"***Error occured reading parameter data***");
       free(store_b);
       return (NULL);
   }

/* after the above process,we should get the m,n */

   m = (int) store_b[3] ;
   n = (int) store_b[4] ;
   if (PrintOut == 1) printf(" Surface has %d X %d patches\n",m,n);
   free(store_b);

/* since we get the m and n,there should be m*n surfaces,for each */
/* sub surface ,we use nx by ny to evaluate it.                   */

   max_data = 6+m+n+48*(m+1)*(n+1);
   store_b = (double *)malloc(max_data*sizeof(double));
   if (store_b == NULL) {
      Error(routine,"Allocation Failed");
      return NULL;
   }

   fseek(pfp,offset,0);
   perf_pro(store_b,max_data);

   surf = get_nurb_surf_from_parametric(store_b,m,n);
   if (surf == NULL)
   {
      Error(routine,"Failed creating NURBS Surface");
      free(store_b);
      return NULL;
   }

   free(store_b);

   if (trans_mtr) transform_surface_cps(surf);
   if (Color != 0) ins_314();

   return (surf);
}

Point *ins_116()
{
    Point *p;
    double  *store_b;
    long   offset;
    char *routine = "ins_116";

    offset = ((parameter - 1 )*RECBUF);
    fseek(pfp,offset,0);

    store_b = (double *) malloc(sizeof(double)*4);
    if (store_b == NULL) {
       Error(routine,"Allocation Failed");
       return((Point *) NULL);
    }
    if (perf_pro(store_b,4) > 0){
        Error(routine,"***Error occured reading parameter data***");
        free(store_b);
        return((Point *)NULL);
    }

    p = (Point *)malloc(sizeof(Point));
    if (p == NULL) {
       Error(routine,"Allocation Failed");
       free(store_b);
       return((Point *)NULL);
    }
    p->x = store_b[1];  /*remember that store_b[0] is the*/
    p->y = store_b[2];  /*entity number                  */
    p->z = store_b[3];

    if (trans_mtr) transform_point_cps(p);
    if (Color != 0) ins_314();

    free(store_b);
    return(p);
}

/*****************************************************************/
/* program : ins_118.c---processes for the ruled surface         */ 
/* ------------------------------------------------------------- */ 
/* ------------------------------------------------------------- */
/* ------ this program assume in the IGES file input ,all the -- */
/* ------ curve segment  should be listed before entype 118 , -- */
/* ------ that means in order to let this program function    -- */
/* ------ properly ,the curve segment  should be done first   -- */
/* ------------------------------------------------------------- */
/* ------------------------------------------------------------- */
/***************  Commented by Robert Yu  ************************/
           
/*****************************************************************/
/*                                                               */
/* Program   : hy_ins_118.c, process for the ruled surface       */
/* Algorithm : search the IGES_NURB_table, find the two basic    */
/*             NURB polygon, if they are of different order and  */
/*             points, evaluate them to two curves with the same */    
/*             reso then re_interpolate them back with higher    */
/*             order thus two polygon will have the same order   */
/*             and the same number of control points             */
/* PassIn    : parameters to read IGES, number of basic elements */
/*             in IGES_NURB_table                                */
/* PassOut   : a control net for ruled_surface                   */   
/* Author    : Yi Hong                                           */
/* Date      : July 11 1992                                      */
/* Remark    : data read from IGES                               */
/*             Index   Name   Type   Description                 */
/*               0     type   real   entity type                 */
/*               1    DE1  pointer ptr to the DE of first entity */
/*               2    DE2  pointer ptr to the DE of second entity*/
/*               3  DIRFLG int     0=join 1 to 1,last to last    */
/*                                 1=join 1 to last, last to 1   */
/*               4  DEVFLG int     1=developable, 0=may not      */
/*****************************************************************/

NurbSurf *ins_118()
{
    NurbSurf  *surf;                  
    NurbCurv  *nurb1, *nurb2;
    long    ptr1,ptr2;                 
    int    order1, order2;   
    int    np_x1,np_x2;
    int    entype1,entype2,type1,type2;
    int    dirflag;
    int    istat;
    int    curv1_id,curv2_id;

    double  *store_b; 
    long    offset;
    DirEntry_t *DE;

    char *routine = "ins_118";
 
/* now,extract the information from the para file for the entype 118 */

    offset = (parameter-1)*RECBUF;
    fseek(pfp,offset,0);
    store_b = (double *) malloc(sizeof(double)*5);
    if (store_b == NULL) {
       Error(routine,"Allocation Failed");
       return((NurbSurf *)NULL);
    }
    if (perf_pro(store_b,5) > 0){
        Error(routine,"***Error occured reading parameter data***");
        free(store_b);
        return((NurbSurf *)NULL);
    }

/* process the de1 and de2 ---the pointer of the curve,could be any  */
/* curve entity ,ex:line,circular arc,conic arc ,parametric curve   */
/* rational B-spline or composit ,determine  entity type  first     */

/* for the first curve */
    ptr1 = (long)store_b[1];

    DE = find_entry(ptr1,1);
    if (DE == NULL) {
       Error(routine,"first entity of ruled surface not found"); 
       free(store_b);
       return((NurbSurf *)NULL);
    }
    type1=DE->type;
    entype1=DE->entype;
    curv1_id=DE->id;         
    nurb1 = (NurbCurv *)DE->ndata;
    if (nurb1 == NULL) {
       Error(routine,"first entity of ruled surface not found"); 
       free(store_b);
       return((NurbSurf *)NULL);
    }

    np_x1 = nurb1->cp_res;
    order1 = nurb1->order;
 
/* for the second curve */
    ptr2 = (long)store_b[2]; 

    DE = find_entry(ptr2,1);
    if (DE == NULL) {
       Error(routine,"second entity of ruled surface not found");
       free(store_b);
       return((NurbSurf *)NULL);
    }
    type2=DE->type;
    entype2=DE->entype;
    curv2_id=DE->id;         
    nurb2 = (NurbCurv *)DE->ndata;
    if (nurb2 == NULL) {
       Error(routine,"first entity of ruled surface not found"); 
       free(store_b);
       return((NurbSurf *)NULL);
    }

    np_x2  = nurb2->cp_res;
    order2 = nurb2->order;

    if ((type1 == POINT) && (type2 == POINT) )
     {
       Error(routine,"Two points can not be formed as a ruled surface !!!");
       free(store_b);
       return((NurbSurf *)NULL); 
     }

    dirflag = (int)store_b[3];

    surf = Ruled_Surface(nurb1,nurb2,dirflag,&istat);
    if (surf == NULL || istat != NO_ERROR){
      free(store_b);
      Error(routine,"Error creating Ruled Surface");
      return NULL;
    }
    if (trans_mtr) transform_surface_cps(surf);
    if (Color != 0) ins_314();
    free(store_b);

    return(surf);
}

/*****************************************************************/
/* program : ins_120.c---processes for the surface of revolution */ 
/* ------------------------------------------------------------- */ 
/* ------------------------------------------------------------- */
/* ------ this program assume in the IGES file input ,all the -- */
/* ------ curve entities should be listed before entype 120 , -- */
/* ------ that means in order to let this program function    -- */
/* ------ properly ,the curve entities should be done first   -- */
/* ------------------------------------------------------------- */
/* -------------------Commented by Robert Yu-------------------- */
/*****************************************************************/
/*                                                               */
/* Program : ins_120.c, process for the body of revolution    */
/* Affiliation : MSU/NSF  ERC for CFS                            */
/* PassIn  : info needed to read IGES, number of elements in the */
/*           IGES_NURB_table                                     */
/* PassOut : a control net for the body of revolution            */
/* Author  : Robert Yu, modified by Yi Hong for ngp              */
/* Date    : Modified on July 10,1992                            */
/* Remark    : data read from IGES                               */
/*             Index   Name   Type   Description                 */
/*               0     type   real   entity type                 */
/*               1       L    pointer pointer to DE of line      */
/*               2       C    pointer pointer to DE of generatrix*/
/*               3      SA    real    starting angle of rotation */
/*               4      TA    real    ending angle of revolution */
/*****************************************************************/

NurbSurf *ins_120()
{
    NurbSurf *surf;
    long      ptr1, ptr2;            
    int      type,entype1;                 
    double   s_angle,t_angle;       
    Point    p1,p2;                
    Hpoint   *p;
    NurbCurv *nurb1,*nurb2;
    double   *store_b;
    long     offset;
    int      istat;
    int      curv1_id,curv2_id;
    DirEntry_t *DE;
    char *routine = "ins_120";

/* now,extract the information from the para file for the entype 120 */

    offset = (parameter-1)*RECBUF;
    fseek(pfp,offset,0);
    store_b = (double *) malloc(sizeof(double)*5);
    if (store_b == NULL) {
       Error(routine,"Allocation Failed");
       return((NurbSurf *)NULL);
    }
    if (perf_pro(store_b,5) > 0){
        Error(routine,"***Error occured reading parameter data***");
        free(store_b);
        return((NurbSurf *)NULL);
    }

    ptr1    = (long)store_b[1]; /* pl & pc :ref IGES 5.0 p100 */ 
    ptr2    = (long)store_b[2]; 
    s_angle = (double)store_b[3];
    t_angle = (double)store_b[4];
    if (t_angle < s_angle)
    {
      s_angle = (double)store_b[4];
      t_angle = (double)store_b[3];
    }

    DE = find_entry(ptr1,1);
    if ((DE == NULL) || (DE->entype != 110)) 
    {
      Error(routine,"axis is not found");
      free(store_b);
      return((NurbSurf *)NULL);
    }
    type=DE->type;
    entype1=DE->entype;
    curv1_id=DE->id;
    nurb1 = (NurbCurv *)DE->ndata;

    p = nurb1->control_point;
    p1.x = p[0].x;
    p1.y = p[0].y;
    p1.z = p[0].z;
    p2.x = p[nurb1->cp_res].x;
    p2.y = p[nurb1->cp_res].y;
    p2.z = p[nurb1->cp_res].z;

/* process the pc--- the pointer of the generatrix,could be any     */
/* curve entity ,ex:line,circle arc,conic arc ,parametric curve     */
/* first of all ,determine which entype does the pc pointed         */

    DE = find_entry(ptr2,1);
    if (DE == NULL)
    {
      Error(routine,"generatrix of revobody not found");
      free(store_b);
      return((NurbSurf *)NULL);
    }
    type=DE->type;
    entype1=DE->entype;
    curv2_id=DE->id; 
    nurb2 = (NurbCurv *)DE->ndata;

    surf = Surface_of_Revolution(nurb2,p1,p2,s_angle,t_angle,&istat);
    if (surf == NULL || istat != NO_ERROR){
      Error(routine,"Error creating Surface of Revolution");
      free(store_b);
      return NULL;
    }
    if (trans_mtr) transform_surface_cps(surf);
    if (Color != 0) ins_314();

    free(store_b);

    return(surf);
}

/*****************************************************************/
/* program : ins_122.c---processes for the tabulated cylinder    */ 
/* ------------------------------------------------------------- */ 
/* ------------------------------------------------------------- */
/* ------ this program assume in the IGES file input ,all the -- */
/* ------ curve segment  should be listed before entype 122 , -- */
/* ------ that means in order to let this program function    -- */
/* ------ properly ,the curve segment  should be done first   -- */
/* ------------------------------------------------------------- */
/* ------------------------------------------------------------- */
/******************** Remark by Robert Yu ************************/
/*****************************************************************/
/*                                                               */
/* Program : ins_122.c, process for the tabulated cylinder    */
/* Affiliation : MSU/NSF  ERC for CFS                            */
/* PassIn  : info needed to read IGES file, number of elements   */
/*           in IGES_NURB_table                                  */
/* PassOut : a control_net for tabulated cylinder                */
/* Author  : Robert Yu, modified by Yi Hong for ngp              */
/* Date    : Modified on July 10,1992                            */
/* Remark    : data read from IGES                               */
/*             Index   Name   Type   Description                 */
/*               0     type   real   entity type                 */
/*               1     DE  pointer   pointer to DE of directrix  */
/*               2     LX    real terminate point.x of generatrix*/
/*               3     LY    real terminate point.y of generatrix*/
/*               4     LZ    real terminate point.z of generatrix*/
/*****************************************************************/

NurbSurf *ins_122()
{
    NurbSurf *surf;
    Point  p1, p2;     
    Hpoint p;
    long    pointer;      
    int    type,entype1;               
    double length;             
    Point  vector;        
    NurbCurv *nurb1;         
    double *store_b;
    long   offset;
    int    istat;
    int    curv_id;
    DirEntry_t *DE;
    char *routine = "ins_122";

    offset = (parameter-1)*RECBUF;
    fseek(pfp,offset,0);
    store_b = (double *) malloc(sizeof(double)*5);
    if (store_b == NULL) {
       Error(routine,"Allocation Failed");
       return((NurbSurf *)NULL);
    }
    if (perf_pro(store_b,5) > 0){
        Error(routine,"***Error occured reading parameter data***");
        free(store_b);
        return((NurbSurf *)NULL);
    }

    pointer     = (long)store_b[1];
    p2.x    = store_b[2]; 
    p2.y    = store_b[3];
    p2.z    = store_b[4];
 
/* process the de--- the pointer of the directrix ,could be any     */
/* curve entity ,ex:line,circular arc,conic arc ,parametric curve   */
/* rational B-spline or composit ,determine  entity type  first     */

    DE = find_entry(pointer,1);
    if (DE == NULL){
       Error(routine,"Entry not found");
       free(store_b);
       return((NurbSurf *)NULL);
    }
    type=DE->type;
    entype1=DE->entype;
    curv_id=DE->id;    
    nurb1 = (NurbCurv *)DE->ndata;
    if (nurb1 == NULL){
       Error(routine,"nurb1 Entry is NULL");
       free(store_b);
       return((NurbSurf *)NULL);
    }

    if (type == NURB_CURVE){
       if (Get3DCurveValueAtU(nurb1,nurb1->knot[nurb1->order-1],&p,0) != 
           NO_ERROR){
          Error(routine,"Error evaluating NurbCurv");
          free(store_b);
          return((NurbSurf *)NULL);
       }
       p1.x = p.x;
       p1.y = p.y;
       p1.z = p.z;
    }
    else{
       Error(routine,"***Error: Directory Entry is not a NURB_CURVE***");
       free(store_b);
       return((NurbSurf *)NULL);
    }

    vector.x = p2.x-p1.x;
    vector.y = p2.y-p1.y;
    vector.z = p2.z-p1.z;
    length = MAG(vector);
    vector.x = vector.x/length;
    vector.y = vector.y/length;
    vector.z = vector.z/length;

    surf = Extruded_Surface(nurb1,vector,length,&istat);
    if (surf == NULL || istat != NO_ERROR){
       Error(routine,"Error generating Extruded_Surface");
       free(store_b);
       return((NurbSurf *)NULL);
    }

    if (trans_mtr) transform_surface_cps(surf);
    if (Color != 0) ins_314();

    free(store_b);

    return(surf);
}

NurbCurv **ins_125()
{
    double  *store_b;
    long   offset;
    NurbCurv *curv[2];
    int istat;
    double twist;
    Point center;
    char *routine = "ins_125";

/* first ,get the information of m and n from the parameter file */

    curv[0] = NULL;
    curv[1] = NULL;

    offset = (parameter - 1 )*RECBUF;
    fseek(pfp,offset,0);

/* get the basic information for the nurbs */
    store_b = (double *) malloc(7*sizeof(double));
    if(store_b == NULL) {
      Error(routine,"Allocation Failed");
      return NULL;
    }
    if (perf_pro(store_b,7) > 0){
       Error(routine,"***Error occured reading parameter data***");
       free(store_b);
       return NULL;
    }

/* store_b[0] : entity number */
/* store_b[1] : X refernce */
/* store_b[2] : Y reference */
/* store_b[3] : First flash sizing parameter */
/* store_b[4] : Second flash sizing parameter */
/* store_b[5] : Rotation of flash about reference point in radians */
/* store_b[6] : Pointer to the DE of the referenced entity or zero */

   center.x = store_b[1];
   center.y = store_b[2];
   center.z = 0.;

   twist = RAD_TO_DEG(store_b[5]);

   switch (form){
      case 0: /* Defined be referenced entity */
         break;
      case 1: /* Circular */
         curv[0] = CircleFromRadiusAndCenter(store_b[3],center,PLANE_XY,0.,
                                             &istat);
         if (curv[0] == NULL || istat != NO_ERROR){
            free(store_b);
            return NULL;
         }
         break;
      case 2: /* Rectangle */
         curv[0] = RectangleToNurbCurv(store_b[3],store_b[4],center,twist,
                                       PLANE_XY,0.,360.,MIDDLE,&istat);
         if (curv[0] == NULL || istat != NO_ERROR){
            free(store_b);
            return NULL;
         }
         break;
      case 3: /* Donut */
         curv[0] = CircleFromRadiusAndCenter(store_b[3],center,PLANE_XY,0.,
                                             &istat);
         if (curv[0] == NULL || istat != NO_ERROR){
            free(store_b);
            return NULL;
         }
         curv[1] = CircleFromRadiusAndCenter(store_b[4],center,PLANE_XY,0.,
                                             &istat);
         if (curv[1] == NULL || istat != NO_ERROR){
            free(store_b);
            return NULL;
         }
         break;
      case 4: /* Canoe */
         curv[0] = OvalToNurbCurv(store_b[3],store_b[4],center,twist,PLANE_XY,
                                  0.,360.,&istat);
         if (curv[0] == NULL || istat != NO_ERROR){
            free(store_b);
            return NULL;
         }
         break;
   }

   free(store_b);

   return curv;
}

NurbCurv *ins_126(int curv_id)
{
   double  *store_b,*data;
   long   offset;
   int    k,m,n,a,i=0,j=0,ii;
   int    max_data;
   NurbCurv *curv;
   Hpoint *p;
   int resid;
   char *routine = "ins_126";

/* first ,get the information of m and n from the parameter file */

   offset = (parameter - 1 )*RECBUF;
   fseek(pfp,offset,0);

/* get the basic information for the nurbs */
   store_b = (double *) malloc(7*sizeof(double));
   if(store_b == NULL) {
     Error(routine,"Allocation Failed");
     return((NurbCurv *)NULL);
   }
   if (perf_pro(store_b,7) > 0){
      Error(routine,"***Error occured reading parameter data***");
      free(store_b);
      return((NurbCurv *)NULL);
   }

/* store_b[0] : entity number */
/* store_b[1] : upper index of sum */
/* store_b[2] : degree of basis function */
/* store_b[3] : 0 = nonplanar, 1 = planar */
/* store_b[4] : 0 = open curve, 1 = closed curve */
/* store_b[5] : 0 = rational, 1 = polynomal */
/* store_b[6] : 0 = nonperiodic, 1 = periodic */

   k  = (int) store_b[1] ;
   m  = (int) store_b[2] ; 
   n  = 1 + k - m        ; 
   a  = n + 2 * m        ;
 
   free(store_b);  	  /* release the memory */
 
 /* allocate a proper memory for the NURB curve   */
 
   max_data = 19+a+4*k;
   data =(double *) malloc(max_data*sizeof(double));
   if (data == NULL) {
      Error(routine,"Allocation Failed");
      return ((NurbCurv *)NULL);
   } 

 /* since we have allocate a proper memory for the array , we can now 
    access the parameter file and store the information to data array */
     
   fseek(pfp,offset,0);
   if((resid = perf_pro(data,max_data)) > 7){
       Error(routine,"***Error occured reading parameter data***");
       free(data);
       return((NurbCurv *)NULL);
   }

   curv = AllocateNurbCurv(curv_id,m+1,k);

   if (curv == NULL){
      Error(routine,"AllocateNurbCurv failed");
      free(data);
      return NULL;
   }

   for (i= 7;i<  8+a;i++)
       curv->knot[i-7] = data[i];

   j = 8+a ;
   for (i=j,ii=0;i<9+a+k;i++,ii++){
     p = &(curv->control_point[ii]);
     p->x = data[i+k+1+2*(i-j)];
     p->y = data[i+k+2+2*(i-j)];
     p->z = data[i+k+3+2*(i-j)];
     p->w = data[i];
   }

   curv->closed = (int)data[4];
   curv->periodic = (int)data[6];

   if (resid == 0)
   {
      curv->u_min = data[12+a+4*k];
      curv->u_max = data[13+a+4*k];
      if (curv->u_min >= curv->u_max)
      {
         curv->u_min = 999.;
         curv->u_max = -999.;
      }
      if (curv->u_min < curv->knot[curv->order-1])
      {
         printf(">>>> u_min out of knot domain\n");
         curv->u_min = curv->knot[curv->order-1];
      }
      if (curv->u_max > curv->knot[curv->cp_res+1])
      {
         printf(">>>> u_max out of knot domain\n");
         curv->u_max = curv->knot[curv->cp_res+1];
      }
   }
   else
   {
      curv->u_min = curv->knot[curv->order-1];
      curv->u_max = curv->knot[curv->cp_res+1];
   }

   if (trans_mtr) transform_curve_cps(curv);
   if (Color != 0) ins_314();
   
   free(data);

   return(curv);
}

NurbSurf *ins_128(int surf_id)
{
   double  *pdata;
   long   offset, max_data;
   int    k1,k2,m1,m2,n1,n2,a,b,c,i=0,j=0,ii,jj;
   int       m;                            /*  Dummy U resolution       */
   int       n;                            /*  Dummy V resolution       */
   int       k;                            /*  Dummy U order            */
   int       l;                            /*  Dummy V order            */
   int    resid;
   NurbSurf *surf = NULL;
   char *routine = "ins_128";
   Hpoint *p;

/* first ,get the information of m and n from the parameter file */

   offset = (parameter - 1)*RECBUF;
   fseek(pfp,offset,0);

/* get the basic information for the nurbs */
   pdata = (double *)malloc(10*sizeof(double));
   if (pdata == NULL) {
      Error(routine,"Allocation Failed");
      return((NurbSurf *)NULL);
   }
   if(perf_pro(pdata,10) > 0){
      Error(routine,"***Error occured reading parameter data***");
      free(pdata);
      return((NurbSurf *)NULL);
   }

/* pdata[0] : entity number */
/* pdata[1] : upper index of first sum */
/* pdata[2] : upper index of second sum */
/* pdata[3] : degree of first set of basis functions */
/* pdata[4] : degree of second set of basis functions */
/* pdata[5] : 1 = Closed in first parametric variable direction
              0 = Not Closed */
/* pdata[6] : 1 = Closed in second parametric variable direction
              0 = Not Closed */
/* pdata[7] : 0 = Rational
              1 = Polynomial */
/* pdata[8] : 0 = Nonperiodic in first parametric variable direction
              1 = Periodic in first parametric variable direction */
/* pdata[9] : 0 = Nonperiodic in second parametric variable direction
              1 = Periodic in second parametric variable direction */

   k1 = (int) pdata[1] ;
   k2 = (int) pdata[2] ;
   m1 = (int) pdata[3] ;
   m2 = (int) pdata[4] ;
   n1 = 1 + k1 - m1    ; 
   n2 = 1 + k2 - m2    ;
   a  = n1+2*m1        ;
   b  = n2+2*m2        ;
   c  = (1+k1)*(1+k2)  ;

   free(pdata);  	  /* release the memory */

/* allocate a proper memory for the NURB surface   */

   max_data = 19+a+b+4*c;
   pdata = (double *)malloc((unsigned int)(max_data*sizeof(double)));
   if (pdata == NULL) {
      Error(routine,"Allocation Failed");
      return((NurbSurf *)NULL);
   }              
/* since we have allocated proper memory for the array , we can now 
   access the parameter file and store the information to data      */

   fseek(pfp,offset,0);
   if((resid = perf_pro(pdata,(int)max_data)) > 7)
   {
        Error(routine,"***Error occured reading parameter data***");
        free(pdata);
        return((NurbSurf *)NULL);
   }

   if (resid == 0)
   {
     if ((int)pdata[max_data-3] == 0)
        if ((int)pdata[max_data-2] == 1)
           name = ins_406((int)pdata[max_data-1]);
   }

/*********************************************************************/
/*  Allocating memory to Nurb Surface data structure and other misc  */
/*  data sets that are used in the algorithm and some initializing.  */
/*********************************************************************/

   surf = AllocateNurbSurf(surf_id,m1+1,m2+1,k1,k2);

   if (surf == NULL){
      Error(routine,"Error allocating Nurb Surface");
      free(pdata);
      return NULL;
   }

   m = surf->cp_res.k;
   n = surf->cp_res.l;
   k = surf->order.k;
   l = surf->order.l;

   for (i=10,ii=0;i < 11+a;i++,ii++)
       surf->knotU[ii] = pdata[i];

   for (j=11+a,jj=0;j < 12+a+b;j++,jj++)
       surf->knotV[jj] = pdata[j];

   ii = 0;
   jj = 0;
   j = 12+a+b ;
   for (i=j;i < (j+c);i++)
   {
     p = &(surf->control_point[ii][jj]);
     p->x = pdata[i+c+2*(i-j)];
     p->y = pdata[i+c+2*(i-j)+1];
     p->z = pdata[i+c+2*(i-j)+2];
     p->w = pdata[i];

     if(ii == m){
        ii=0;
        jj++;
     }
     else
        ii++;
   }

   surf->closed_in_u = (int)pdata[5];
   surf->closed_in_v = (int)pdata[6];
   surf->periodic_in_u = (int)pdata[8];
   surf->periodic_in_v = (int)pdata[9];

   if (resid <= 1)
   {
      surf->u_min = pdata[12+a+b+4*c];
      surf->u_max = pdata[13+a+b+4*c];
      if (surf->u_min >= surf->u_max)
      {
         surf->u_min = 999.;
         surf->u_max = -999.;
      }
      surf->v_min = pdata[14+a+b+4*c];
      surf->v_max = pdata[15+a+b+4*c];
      if (surf->v_min >= surf->v_max)
      {
         surf->v_min = 999.;
         surf->v_max = -999.;
      }
      if (surf->u_min < surf->knotU[surf->order.k-1])
      {
         printf(">>>> umin out of knot domain\n");
         surf->u_min = surf->knotU[surf->order.k-1];
      }
      if (surf->u_max > surf->knotU[surf->cp_res.k+1])
      {
         printf(">>>> umax out of knot domain\n");
         surf->u_max = surf->knotU[surf->cp_res.k+1];
      }
      if (surf->v_min < surf->knotV[surf->order.l-1])
      {
         printf(">>>> vmin out of knot domain\n");
         surf->v_min = surf->knotV[surf->order.l-1];
      }
      if (surf->v_max > surf->knotV[surf->cp_res.l+1])
      {
         printf(">>>> vmax out of knot domain\n");
         surf->v_max = surf->knotV[surf->cp_res.l+1];
      }
   }
   else
   {
      surf->u_min = surf->knotU[surf->order.k-1];
      surf->u_max = surf->knotU[surf->cp_res.k+1];
      surf->v_min = surf->knotV[surf->order.l-1];
      surf->v_max = surf->knotV[surf->cp_res.l+1];
   }

   if (trans_mtr) transform_surface_cps(surf);
   if (Color != 0) ins_314();

   free(pdata);
   return(surf);
}

NurbCurv *ins_130()
{
    NurbCurv *curv = NULL;
    double   *store_b;
    long     offset;
    char *routine = "ins_130";

/* now,extract the information from the para file for the entype 130 */

    offset = (parameter-1)*RECBUF;
    fseek(pfp,offset,0);
    store_b = (double *) malloc(sizeof(double)*15);
    if (store_b == NULL) {
       Error(routine,"Allocation Failed");
       return NULL;
    }
    if (perf_pro(store_b,15) > 0){
        Error(routine,"***Error occured reading parameter data***");
        free(store_b);
        return NULL;
    }
    /* store_b[ 1] Pointer to curve to be offset              */
    /* store_b[ 2] Offset distance flag                       */
    /*        1 = Single value Offset, uniform                */
    /*        2 = Offset distance varies linearly             */
    /*        3 = Offset distance as a specified function     */
    /* store_b[ 3] Pointer to curve which describes function  */
    /* store_b[ 4] Coordinate of curve of function            */
    /* store_b[ 5] Tapered offset type flag                   */
    /*        1 = Function of arc length                      */
    /*        2 = Function of parameter                       */
    /* store_b[ 6] First offset distance (flag = 1 or 2)      */
    /* store_b[ 7] Function of first  offset distance         */
    /* store_b[ 8] Second offset distance                     */
    /* store_b[ 9] Function of second offset distance         */
    /* store_b[10] X-component of unit vector normal to plane */
    /* store_b[11] Y-component of unit vector normal to plane */
    /* store_b[12] Z-component of unit vector normal to plane */
    /* store_b[13] Offset curve starting parameter value      */
    /* store_b[14] Offset curve ending   parameter value      */

    return(curv);
}

NurbSurf *ins_140()
{
    NurbSurf *surf = NULL,*ns;
    double   *store_b;
    long     offset;
    DirEntry_t *DE;
    int      istat;
    char *routine = "ins_140";

/* now,extract the information from the para file for the entype 140 */

    offset = (parameter-1)*RECBUF;
    fseek(pfp,offset,0);
    store_b = (double *) malloc(sizeof(double)*6);
    if (store_b == NULL) {
       Error(routine,"Allocation Failed");
       return NULL;
    }
    if (perf_pro(store_b,6) > 0){
        Error(routine,"***Error occured reading parameter data***");
        free(store_b);
        return NULL;
    }
    /* store_b[1] X-coordinate of offset indicator */
    /* store_b[2] Y-coordinate of offset indicator */
    /* store_b[3] Z-coordinate of offset indicator */
    /* store_b[4] Distance                         */
    /* store_b[5] Pointer to surface entity        */

    
    DE = find_entry((long)store_b[5],1);
    if (DE == NULL)
    {
       Error(routine,"Entry not found");
       free(store_b);
       return (NULL);
    }
    ns = (NurbSurf *)DE->ndata;

    surf = OffsetSurf(ns,store_b[4],0,&istat);
    free(store_b);
    if (surf == NULL || istat != NO_ERROR)
    {
       Error(routine,"Offset surface failed");
       return NULL;
    }

    if (trans_mtr) transform_surface_cps(surf);
    if (Color != 0) ins_314();

    return surf;
}

NurbCurv *ins_142()
{
    double  *store_b;
    long   offset;
    NurbCurv *curv;
    NurbSurf *surf;
    DirEntry_t *DE, *sDE, *cDE;
    int scurv_id = -1;
    Hpoint *cp;
    int i;
    double umin,umax,vmin,vmax;
    char *routine = "ins_142";

/* first ,get the information of m and n from the parameter file */

    offset = (parameter - 1 )*RECBUF;
    fseek(pfp,offset,0);

/* get the basic information for the nurbs */
    store_b = (double *) malloc(6*sizeof(double));
    if (store_b == NULL)
    {
       Error(routine,"Allocation Failed");
       return NULL;
    }
    if (perf_pro(store_b,6) > 0)
    {
       Error(routine,"***Error occured reading parameter data***");
       free(store_b);
       return NULL;
    }

/* store_b[0] : entity number */
/* store_b[1] : indicates the way the curve on the surface has been created
                0 = Unspecified
                1 = Projection of a given curve on the surface
                2 = Intersection of two surfaces
                3 = Isoparametric curve, i.e. either a u or v-parametric */
/* store_b[2] : Pointer to DE of the surface on which the curve lies */
/* store_b[3] : Pointer to DE of the entity that contains the definition of
                the curve B in the parametric space (u,v) of the surface */
/* store_b[4] : Pointer to the DE of the curve C */
/* store_b[5] : Indicates preferred representation in the sending system:
                0 = Unspecified
                1 = S o B is preferred
                2 = C is preferred
                3 = C and S o B are equally preferred */

    DE = find_entry((long)store_b[3],1);
    if (DE == NULL || (DE->type != NURB_CURVE && DE->type != PARAMETRIC_CURVE))
    {
       Error(routine,"Entry not found");
       free(store_b);
       return NULL;
    }
    if (DE->process == 1)
    {
       if (PrintOut == 1) printf("Curve will be deleted\n");
       return NULL; 
    }
    curv = (NurbCurv *)DE->ndata;
    if (curv == NULL)
    {
       if (PrintOut == 1) printf("Curve not found\n");
       return NULL;
    }

    sDE = find_entry((long)store_b[2],1);
    if (sDE == NULL || sDE->type != NURB_SURFACE)
    {
       Error(routine,"Entry not found");
       free(store_b);
       return NULL;
    }
    surf = (NurbSurf *)sDE->ndata;
    if (surf == NULL)
    {
       Error(routine,"Parent surface not found");
       free(store_b);
       return NULL;
    }
    DE->parent_entype = sDE->entype;

    umin = surf->knotU[surf->order.k-1];
    umax = surf->knotU[surf->cp_res.k+1];

    vmin = surf->knotV[surf->order.l-1];
    vmax = surf->knotV[surf->cp_res.l+1];

    for(i=0;i<=curv->cp_res;i++)
    {
       cp = &curv->control_point[i];

       cp->x = (cp->w*cp->x-umin)/(umax-umin)/cp->w;
       cp->y = (cp->w*cp->y-vmin)/(vmax-vmin)/cp->w;
    }

    cDE = find_entry((long)store_b[4],1);
    if (cDE == NULL || cDE->type != NURB_CURVE)
    {
       Error(routine,"Entry not found");
       curv->space_curv_ptr = NULL;
    }
    else
    {
       scurv_id = cDE->id;
       cDE->seqnum = cDE->id = cDE->entype = cDE->type = 0;
       curv->space_curv_ptr = (NurbCurv *)cDE->ndata;
       cDE->process = 1;
    }
    free(store_b);

    DE->process = 1;
    DE->visible = 2;

    curv->parent_surf_ptr = surf;

    return curv;
}

NurbCurv *unioned_curves(menuCalldata *info,NurbCurv *nc1,NurbCurv *nc2)
{
   char *routine = "unioned_curves";
   Hpoint v1,v2,normal;
   NurbCurv *NC[2];
   NurbCurv *nc = NULL;
   int istat;
   Edge *Edge_PTR;

   Get3DCurveValueAtU(nc1,nc1->knot[nc1->order-1],&v1,1);
   Get3DCurveValueAtU(nc2,nc2->knot[nc2->cp_res+1],&v2,1);
   NORMALIZE(v1);
   NORMALIZE(v2);
   CROSS_PRODUCT(v1,v2,normal);

   if (MAG(normal) > .00005) return NULL;

   NC[0] = nc1;
   NC[1] = nc2;
   nc = UnionMultipleCurves(NC,2,NULL,0,&istat);

   if (nc == NULL || istat != NO_ERROR) 
   {
      Edge_PTR = nc1->edge_ptr;
      if(Edge_PTR == NULL)
      {
        Error(routine,"NULL Edge pointer");
        return NULL;
      }
      if(Edge_PTR->EdgeUse_PTR == Edge_PTR->EdgeUse_PTR->next_EdgeUse_PTR)
      {
        if(DeleteEdge(info,Edge_PTR) == TRUE)
        {
          LoudError(info,"Unable to delete Edge");
        }
      }

      Edge_PTR = nc2->edge_ptr;
      if(Edge_PTR == NULL)
      {
        Error(routine,"NULL Edge pointer");
        return NULL;
      }
      if(Edge_PTR->EdgeUse_PTR == Edge_PTR->EdgeUse_PTR->next_EdgeUse_PTR)
      {
        if(DeleteEdge(info,Edge_PTR) == TRUE)
        {
          LoudError(info,"Unable to delete Edge");
        }
      }
   }

   return nc;
}

Boolean triangular_region(NurbCurv **nc,Hpoint *p)
{
   Hpoint v1,v2;
   double angle1,angle2,angle3;

   Get3DCurveValueAtU(nc[0],nc[0]->knot[nc[0]->cp_res+1],&v1,1);
   Get3DCurveValueAtU(nc[1],nc[1]->knot[nc[1]->order-1],&v2,1);
   NORMALIZE(v1);
   NORMALIZE(v2);
   angle1 = acos(DOT_PRODUCT(v1,v2));

   Get3DCurveValueAtU(nc[1],nc[1]->knot[nc[1]->cp_res+1],&v1,1);
   Get3DCurveValueAtU(nc[2],nc[2]->knot[nc[2]->order-1],&v2,1);
   NORMALIZE(v1);
   NORMALIZE(v2);
   angle2 = acos(DOT_PRODUCT(v1,v2));

   Get3DCurveValueAtU(nc[2],nc[2]->knot[nc[2]->cp_res+1],&v1,1);
   Get3DCurveValueAtU(nc[0],nc[0]->knot[nc[0]->order-1],&v2,1);
   NORMALIZE(v1);
   NORMALIZE(v2);
   angle3 = acos(DOT_PRODUCT(v1,v2));

   if (angle1+angle2+angle3 > 1.6*PI) return FALSE;

   if (angle1 < angle2 && angle1 < angle3)
      Get3DCurveValueAtU(nc[0],nc[0]->knot[nc[0]->cp_res+1],p,0);
   else if (angle2 < angle3)
      Get3DCurveValueAtU(nc[1],nc[1]->knot[nc[1]->cp_res+1],p,0);
   else
      Get3DCurveValueAtU(nc[2],nc[2]->knot[nc[2]->cp_res+1],p,0);

   return TRUE;
}

#if 0
NurbSurf *ins_144(menuCalldata *info)
{
   double  *pdata;
   long   offset;
   NurbCurv **in_curv;
   NurbCurv **out_curv;
   NurbCurv **curv = NULL;
   NurbCurv *bcurv = NULL;
   NurbSurf *psurf = NULL;
   NurbSurf *surf = NULL;
   Face *Face_PTR;
   Edge **Edge_PTR;
   int num_out = 0,num_in = 0;
   int num_out = 0,num_in = 0;
   int n1,n2,i;
   int istat = NO_ERROR;
   DirEntry_t *DE, *tDE;
   char *routine = "ins_144";



/* first ,get the information of m and n from the parameter file */

   offset = (parameter - 1)*RECBUF;
   fseek(pfp,offset,0);

/* get the basic information for the nurbs */
   pdata = (double *)malloc(4*sizeof(double));
   if(pdata == NULL)
   {
     Error(routine,"Allocation Failed");
     return((NurbSurf *)NULL);
   }
   if(perf_pro(pdata,4) > 0)
   {
      Error(routine,"***Error occured reading parameter data***");
      free(pdata);
      return((NurbSurf *)NULL);
   }

/* pdata[0] : entity number */
/* pdata[1] : Pointer to DE of surface to be trimmed */
/* pdata[2] : 0 = Outer boundary is D, 1= otherwise */
/* pdata[3] : Number of simple closed curves of inner boundary */
/* pdata[4] : Pointer to DE of the first simple closed curve */
/* pdata[...] : Pointer to DE of the ... simple closed curve */

   n1 = (int)pdata[2];
   n2 = (int)pdata[3];

   free(pdata);

   if (n1+n2 != 0)
   {
      pdata = (double *)malloc((n1+n2+4)*sizeof(double));
      if(pdata == NULL)
      {
        Error(routine,"Allocation Failed");
        return NULL;
      }
      fseek(pfp,offset,0);
      if(perf_pro(pdata,n1+n2+4) > 0)
      {
         Error(routine,"***Error occured reading parameter data***");
         free(pdata);
         return NULL;
      }
   }
   else
   {
      return NULL;
   }

   tDE = find_entry((long)pdata[1],1);
   if(tDE == NULL)
   {
     Error(routine,"entry not found");
     free(pdata);
     return NULL;
   }
   psurf = tDE->ndata;
   NormalizeSurfaceDomain(psurf);

   if (psurf->face_ptr == NULL)
      Face_PTR = AddSurface(info,psurf,NURB_SURFACE);

   Edge_PTR = (Edge **)malloc((n1+n2)*sizeof(Edge *));
   if (n2 != 0)
   {
      curv = (NurbCurv **)malloc(n2*sizeof(NurbCurv *));
      if(curv == NULL)
      {
         free(Edge_PTR);
         free(pdata);
         Error(routine,"Allocation Failed");
         return NULL;
      }

      for (i=0;i<n2;i++)
      {
         DE = find_entry((long)pdata[i+5],1);
         if (DE == NULL)
         { 
            Error(routine,"entry not found");
            free(pdata); free(curv); free(Edge_PTR);
            return NULL;
         }
         curv[i] = (NurbCurv *)DE->ndata;
         curv[i]->parent_surf_ptr = psurf;
         if (curv[i]->edge_ptr == NULL)
            Edge_PTR[i] = AddCurve(info,curv[i],PARAMETRIC_CURVE);
         else
            Edge_PTR[i] = curv[i]->edge_ptr;
      }
   }

   if (n1 == 1)
   {
      DE = find_entry((long)pdata[4],1);
      if(DE == NULL)
      {
        Error(routine,"entry not found");
        free(pdata); free(Edge_PTR);
        return NULL;
      }
      bcurv = (NurbCurv *)DE->ndata;
      if (bcurv->edge_ptr == NULL)
         Edge_PTR[n2] = AddCurve(info,bcurv,PARAMETRIC_CURVE);
      else
         Edge_PTR[n2] = bcurv->edge_ptr;
      
   }

   free(pdata);

   if (CreateTrimmedFace(info,Face_PTR,n1+n2,Edge_PTR) == TRUE)
   {
   }

   free(Edge_PTR);
   free(curv);

   if (n1 == 1 && n2 == 1)
   {
      num_out = SplitNurbCurveAtDiscontinuities(info,bcurv,&out_curv);
      num_in = SplitNurbCurveAtDiscontinuities(info,curv[0],&in_curv);
      if (num_in == 0 && num_out == 0)
      {
         double umin,umax;
         Hpoint p;

         bcurv->closed = 1;
         curv[0]->closed = 1;

         bcurv->parent_surf_ptr = NULL;
         curv[0]->parent_surf_ptr = NULL;

         Get3DCurveValueAtU(bcurv,bcurv->knot[bcurv->order-1],&p,0);
         umin = ClosestUOnCurve(&p,curv[0],curv[0]->knot[curv[0]->order-1]);
         umax = ClosestUOnCurve(&p,curv[0],curv[0]->knot[curv[0]->cp_res+1]);
         if ((umin == curv[0]->knot[curv[0]->order-1] ||
              umin == curv[0]->knot[curv[0]->cp_res+1]) &&
             (umax == curv[0]->knot[curv[0]->order-1]  ||
              umax == curv[0]->knot[curv[0]->cp_res+1]))
         {
            surf = Ruled_Surface(bcurv,curv[0],2,&istat);
            if (surf == NULL || istat != NO_ERROR)
            {
               Error(routine,"Failed creating ruled trimmed surface");
            }
         }
         bcurv->parent_surf_ptr = psurf;
         curv[0]->parent_surf_ptr = psurf;
      }
      if (num_out > 0)
      {
         DE = find_entry((long)pdata[4],1);
         DE->process = 1;
      }
      if (num_in > 0)
      {
         DE = find_entry((long)pdata[5],1);
         DE->process = 1;
      }
   }
   else if (n1 == 1 && n2 == 0)
   {
      num_out = SplitNurbCurveAtDiscontinuities(info,bcurv,&out_curv);
      if (num_out == 5)
      {
         NurbCurv *unc;

         for(i=0;i<num_out;i++)
            out_curv[i]->parent_surf_ptr = NULL;
         if ((unc = unioned_curves(info,out_curv[0],out_curv[4])) != NULL)
         {
            out_curv[0] = unc;
            num_out--;
            surf = TFI(out_curv[0],out_curv[1],out_curv[2],out_curv[3],NULL,
                       TFI_ARCLENGTH,0,0.,&istat);
            if (surf == NULL || istat != NO_ERROR)
            {
               Error(routine,"Failed creating four sided trimmed surface");
            }
            else
            {
               DE = find_entry((long)pdata[4],1);
               DE->process = 1;
            }
         }
      }
      else if (num_out == 4)
      {
         for(i=0;i<num_out;i++)
            out_curv[i]->parent_surf_ptr = NULL;
         surf = TFI(out_curv[0],out_curv[1],out_curv[2],out_curv[3],NULL,
                    TFI_ARCLENGTH,0,0.,&istat);
         if (surf == NULL || istat != NO_ERROR)
         {
            Error(routine,"Failed creating four sided trimmed surface");
         }
         else
         {
            DE = find_entry((long)pdata[4],1);
            DE->process = 1;
         }
      }
      else if (num_out == 3)
      {
         Hpoint degen_point;

         for(i=0;i<num_out;i++)
            out_curv[i]->parent_surf_ptr = NULL;
         if (triangular_region(out_curv,&degen_point) == TRUE)
         {
            surf = TFI(out_curv[0],out_curv[1],out_curv[2],NULL,&degen_point,
                       TFI_ARCLENGTH,0,0.,&istat);
            if (surf == NULL || istat != NO_ERROR)
            {
               Error(routine,"Failed creating triangular trimmed surface");
            }
            else
            {
               DE = find_entry((long)pdata[4],1);
               DE->process = 1;
            }
         }
         else
         {
            DE = find_entry((long)pdata[4],1);
            DE->process = 1;
         }
      }
      else if (num_out > 0)
      {
         DE = find_entry((long)pdata[4],1);
         DE->process = 1;
      }
   }

   if (n1 == 1)
   {
      if (num_out > 0)
      {
         for(i=0;i<num_out;i++)
            out_curv[i]->parent_surf_ptr = psurf;
      }
      if (num_out > 0) free(out_curv);
   }

   if (n2 > 1)
   {
      if (num_in > 0)
      {
         for(i=0;i<num_in;i++)
            in_curv[i]->parent_surf_ptr = psurf;
      }
      if (num_in > 0) free(in_curv);
      if (curv != NULL) free(curv);
   }

   if (surf == NULL || istat != NO_ERROR)
   {
      Error(routine,"Failed creating trimmed surface");
      tDE->process = 0;
      tDE->visible = 0;
      psurf->Delete = 0;
      free(pdata);
      return NULL;
   }

   tDE->process = 1;
   tDE->visible = 2;

   surf->parent_surf_ptr = psurf;
   psurf->Delete = 1;

   free(pdata);

   return surf;

   return NULL;
}
#endif

void ins_314()
{
   char buf[RECBUF];
   double *data;
   long offset;
   char *routine = "ins_314";
 
   if (Color > 0)
   {
      switch(Color)
      {
         case 1:
           red = green = blue = 0.;
           break;
         case 2:
           red = 100.;
           green = blue = 0.;
           break;
         case 3:
           green = 100.;
           red = blue = 0.;
           break;
         case 4:
           red = green = 100.;
           blue = 0.;
           break;
         case 5:
           blue = 100.;
           red = green = 0.;
           break;
         case 6:
           red = blue = 100.;
           green = 0.;
           break;
         case 7:
           green = blue = 100.;
           red = 0.;
           break;
         case 8:
           red = green = blue = 100.;
           break;
      }
      return;
   }
  
   offset = (-Color-1)*RECBUF;
   fseek(dfp,offset,0);
   fread(buf,sizeof(char),RECBUF,dfp);
   parameter = field_num(buf,2);
   fseek(dfp,savep,0);    

   offset = (parameter-1)*RECBUF;
   fseek(pfp,offset,0);
   data = (double *)malloc(4*sizeof(double));
   if(data == NULL) {
     Error(routine,"Allocation  Failed");
     return;
   }
   if (perf_pro(data,4) > 0){
       Error(routine,"***Error occured reading parameter data***");
       free(data);
       return;
   }

   red = data[1];
   green = data[2];
   blue = data[3];

   free(data);

   return;
}

Group *ins_402(menuCalldata *info)
{
   char buf[RECBUF];
   Group *group = NULL;
   DirEntry_t *DE;
   double *data;
   int num_entries,n,left;
   long offset;
   char *routine = "ins_402";
   
   offset = (seqnum-1)*RECBUF;
   fseek(dfp,offset,0);
   fread(buf,sizeof(char),RECBUF,dfp);
   parameter = field_num(buf,2);
   fread(buf,sizeof(char),RECBUF,dfp);
   form      = field_num(buf,5);

   offset = (parameter-1)*RECBUF;
   fseek(pfp,offset,0);
   data = (double *)malloc(2*sizeof(double));
   if(data == NULL) {
     Error(routine,"Allocation  Failed");
     return group;
   }
   if (perf_pro(data,2) > 0){
       Error(routine,"***Error occured reading parameter data***");
       free(data);
       return group;
   }
   num_entries = (int)data[1];
   free(data);
   fseek(pfp,offset,0);
   data = (double *)malloc((num_entries+5)*sizeof(double));
   if(data == NULL) {
     Error(routine,"Allocation Failed");
     return group;
   }
   if ((left = perf_pro(data,(num_entries+5))) > 3)
   {
     Error(routine,"***Error occured reading parameter data***");
     free(data);
     return group;
   }

   if (left == 0)
   {
      if ((int)data[num_entries+2] == 0)
      {
         if ((int)data[num_entries+3] == 1)
         {
            name = ins_406((int)data[num_entries+4]);
         }
      }
   }

   group = CreateGroup(info,name);
   if (group == NULL){
      Error(routine,"Error creating group");
      free(data);
      return group;
   }

   for (n=0;n<num_entries;n++){
      DE = find_entry((long)data[2+n],0);
      if(DE == NULL || DE->data == NULL) continue;
      switch(DE->type)
      {
         case VECTOR:
            if (InsertVectorIntoGroup(info,group,(Vector *)(DE->data)))
            {
               Error(routine,"Inserting Vector into group failed");
               free(data);
               return group;
            }
            break;
         case VERTEX:
            if (InsertVertexIntoGroup(info,group,(Vertex *)(DE->data)))
            {
               Error(routine,"Inserting Vertex into group failed");
               free(data);
               return group;
            }
            break;
         case EDGE:
            if (InsertEdgeIntoGroup(info,group,(Edge *)(DE->data)))
            {
               Error(routine,"Inserting Edge into group failed");
               free(data);
               return group;
            }
            break;
         case FACE:
            if (InsertFaceIntoGroup(info,group,(Face *)(DE->data)))
            {
               Error(routine,"Inserting Face into group failed");
               free(data);
               return group;
            }
            break;
      }
   }
   free(data);

   return group;
}

/* Read the name of the entity */
char *ins_406(int denum)
{
   long offset;
   int param,frm,i=0,j,k,l;
   char buf[RECBUF],tmp[30];
   char *name = NULL;
   
   offset = (denum-1)*RECBUF;
   fseek(dfp,offset,0);
   fread(buf,sizeof(char),RECBUF,dfp);
   param   = (int)field_num(buf,2);
   fread(buf,sizeof(char),RECBUF,dfp);
   frm     = (int)field_num(buf,5);
   fseek(dfp,savep,0);    

   switch (frm)
   {
       case 15:
              offset = (param-1)*RECBUF;
              fseek(pfp,offset,0);
              fread(buf,sizeof(char),RECBUF,pfp);
              while(buf[i] != record_d && i<RECBUF){
                if (buf[i] == field_d) j = i;
                if (buf[i] == 'H'){
                   for(k=0,l=j+1;l<i;l++,k++) tmp[k] = buf[l]; tmp[k] = '\0';
                   name = (char *)malloc((atoi(tmp)+1)*sizeof(char));
                   for(k=0,l=i+1;k<atoi(tmp);k++,l++) name[k] = buf[l];
                   name[k] = '\0';
                   return name;
                }
                i++;
              }
              break;
       default:
              break;
   }

   return name;
}

void ins_5001(int *lid,int *sid,int curv_id,int surf_id,void **VP)
{
    NurbSurf *surf = NULL;
    NurbCurv *curv = NULL;
    Point **vp = NULL;
    int *np = NULL;
    double  *store_b = NULL;
    Point **data = NULL,*data2 = NULL;
    long   offset;
    int    i,j,n,nn,left;
    int    size_men, ist;
    int    dim,nc,nt; /* dim=dimension flag,nc=# of curve,nt=tot pts */
    int    nx,ny; /* number of pts on I and J direciton  */
    double  zt;
    static int ends[2] = {OPEN, OPEN};
    Boolean variable = FALSE;
    char *routine = "ins_5001";

    VP[0] = (void *)NULL;

/* first , get the information of n from the parameter file
   we do this to ensure to have proper dimension for store_b */

    offset = (parameter - 1)*RECBUF;
    fseek(pfp,offset,0);

    store_b=(double *) malloc(3*sizeof(double)); 
    if(store_b == NULL) {
      Error(routine,"Allocation Failed");
      return;
    }
    if (perf_pro(store_b,3) > 0){
       Error(routine,"***Error occured reading parameter data***");
       free(store_b);
       return;
    }

/* store_b[0] : entity number */
/* store_b[1] : 2 = triad */
/* store_b[2] : number of rows */

    dim  = (int) store_b[1]; 
    nc   = (int) store_b[2]; 
    free(store_b) ;

/* store_b[3] : number of points in row 1 */
/* store_b[4] : number of points in row 2 */
/* store_b[...] : number of points in row ... */
/* store_b[nc+2] : number of points in row nc */
/* store_b[nc+3] : total number of points */
/* store_b[nc+4] : constant z */

/* this routine assumes that there are an equal number of points in each row */

    store_b=(double *) malloc((5+nc)*sizeof(double)); 
    if (store_b == NULL) {
       Error(routine,"Allocation Failed");
       return;
    }
    fseek(pfp,offset,0);
    if (perf_pro(store_b,nc+5) > 0){
       Error(routine,"***Error occured reading parameter data***");
       free(store_b);
       return;
    }
    nt   = (int) store_b[nc+3]   ;
    zt   =       store_b[nc+4]   ;
    ny   =  nc; 
    nx   =  nt / nc;

    for (n=1;n<=nc;n++)
       if (nx != (int)store_b[n+2])
          variable = TRUE;

    free(store_b) ;

    if (dim == 2)
       size_men = nc+5+3*nt;
    else
       size_men = nc+5+2*nt;
    fseek(pfp,offset,0);
    store_b = (double *) malloc((size_men+3)*sizeof(double));
    if(store_b == NULL) {
      Error(routine,"Allocation Failed");
      return;
    }

    if ((left = perf_pro(store_b,size_men+3)) > 3){
        Error(routine,"***Error occured reading parameter data***");
        free(store_b);
        return;
    }

    if (left == 0)
    {
      if ((int)store_b[size_men] == 0)
         if ((int)store_b[size_men+1] == 1)
            name = ins_406((int)store_b[size_men+2]);
    }

    if (ny == 1){
       *lid = 1;
       data2 = (Point *)malloc(nx*sizeof(Point));
       if (data2 == NULL) {
          Error(routine,"Allocation Failed");
          free(store_b);
          return;
       }
       if (dim == 2 )
       {
          for (n=nc+5,nn=1,i=0;nn<=nx;n+=3,i++,nn++){
               data2[i].x=store_b[n] ;
               data2[i].y=store_b[n+1] ;
               data2[i].z=store_b[n+2] ;
          }
       }
       else
       {
          for (n=nc+5,nn=1,i=0;nn<=nx;n+=2,nn++,i++)
          {
               data2[i].x=store_b[n]   ;
               data2[i].y=store_b[n+1] ;
               data2[i].z=zt           ;
	   }
       }
   
#if 1
       curv = LinearCurveInterpolation(nx-1,data2,OPEN,CHORDLENGTH,curv_id,
                                       &ist);
#else
       curv = BJ_Interpolate_3D_curve_points(nx-1,data2,OPEN,CHORDLENGTH,4,
                                             curv_id,&ist);
#endif
       if(curv == NULL || ist != NO_ERROR) {
         Error(routine,"Interpolation of 3D curve points failes");
         free(store_b); free(data2);
         return;
       } 
       if (trans_mtr) transform_curve_cps(curv);
       if (Color != 0) ins_314();

       VP[0] = (void *)curv;

       free(data2);
    }
    else if (variable == FALSE){
       *sid = 1;
       data = (Point **)malloc(nx*sizeof(Point *));
       if (data == NULL){
          Error(routine,"Allocation Failed");    
          free(store_b);
          return;
       }
          for(i=0; i<nx; i++){
             data[i] = (Point *)malloc(ny*sizeof(Point));
             if(data [i] == NULL){
               Error(routine,"Allocation Failed");
               for(j=i-1;j>=0; j--)
                   free(data[j]);
               free(data);  free(store_b);
             }
          }
                  
       if (dim == 2 )
       {
          for (n=nc+5,nn=1,i=0,j=0;nn<=nt;n+=3,i++,nn++){
               if ( i == nx ){
                  i = 0;
                  j++;
               }
               data[i][j].x=store_b[n] ;
               data[i][j].y=store_b[n+1] ;
               data[i][j].z=store_b[n+2] ;
          }
       }
       else
       {
          for (n=nc+5,nn=1,i=0,j=0;nn<=nt;n+=2,nn++,i++)
          {
               if ( i == nx ){
                  i = 0;
                  j++;
               }
               data[i][j].x=store_b[n]   ;
               data[i][j].y=store_b[n+1] ;
               data[i][j].z=zt           ;
	   }
       }
   
       surf = BJ_Interpolate_3D_surf_points(nx-1,ny-1,data,ends,CHORDLENGTH,surf_id,&ist);
       if(surf != NULL && ist == NO_ERROR){
          if (trans_mtr) transform_surface_cps(surf);
          if (Color != 0) ins_314();
          surf->clip_flag = 1;
          VP[0] = (void *)surf;
       }
       else
           Error(routine,"Error interpolaitng 3D surf points");


       for (i=0;i<nx;i++) 
          free(data[i]);
       free(data);
    }
    else{
       *sid = 1;
       vp = (Point **)malloc(nc*sizeof(Point *));
       np = (int *)malloc(nc*sizeof(int));
       if (vp == NULL || np == NULL){
          Error(routine,"Allocation Failed");    
          if (vp) free(vp);
          if (np) free(np);
          free(store_b);
          return;
       }
       for(i=0; i<nc; i++){
          np[i] = (int)store_b[3+i];
          vp[i] = (Point *)malloc(np[i]*sizeof(Point));
          if(vp[i] == NULL){
            Error(routine,"Allocation Failed");
            for(j=i-1;j>=0; j--)
                free(vp[j]);
            free(vp);
            free(np);
            free(store_b);
          }
       }
                  
       if (dim == 2 )
       {
          for (n=nc+5,i=0;i<nc;i++)
            for (j=0;j<np[i];n+=3,j++){
               vp[i][j].x=store_b[n] ;
               vp[i][j].y=store_b[n+1] ;
               vp[i][j].z=store_b[n+2] ;
            }
       }
       else
       {
          for (n=nc+5,i=0;i<nc;i++)
            for (j=0;j<np[i];n+=2,j++){
               vp[i][j].x=store_b[n]   ;
               vp[i][j].y=store_b[n+1] ;
               vp[i][j].z=zt           ;
  	    }
       }
   
       surf = GetSurfaceFromXSectionPoints(nc,np,vp);
       if(surf != NULL){
          if (trans_mtr) transform_surface_cps(surf);
          if (Color != 0) ins_314();
          VP[0] = (void *)surf;
       }
       else
           Error(routine,"Error interpolaitng 3D surf points");

       for (i=0;i<nc;i++) 
          free(vp[i]);
       free(vp); free(np);
    }
    free(store_b);
    return;
}

/* Process Parametric Surface data */
/* This is not a standard entity */

NurbSurf *ins_7366()
{
    double  *store_b;
    long   offset;
    NurbSurf *psurf;
    NurbSurf *surf;
    DirEntry_t *DE, *tDE;
    int Delete;
    char *routine = "ins_7366";

/* first ,get the information of m and n from the parameter file */

    offset = (parameter - 1 )*RECBUF;
    fseek(pfp,offset,0);

/* get the basic information for the nurbs */
    store_b = (double *) malloc(4*sizeof(double));
    if (store_b == NULL){
       Error(routine,"Allocation Failed");
       return NULL;
    }
    if (perf_pro(store_b,4) > 0){
       Error(routine,"***Error occured reading parameter data***");
       free(store_b);
       return NULL;
    }

/* store_b[0] : entity number */
/* store_b[1] : indicates if parent surface is active or is replaced
                0 = Active
                1 = Not Active (replace) */
/* store_b[2] : Pointer to DE of the surface on which the surface lies */
/* store_b[3] : Pointer to DE of the entity that contains the definition of
                the surface B in the parametric space (u,v) of the surface */

    Delete = (int)store_b[1];

    DE = find_entry((long)store_b[3],1);
    if (DE == NULL){
       Error(routine,"Entry not found");
       free(store_b);
       return NULL;
    }
    psurf = (NurbSurf *)DE->ndata;
    if (psurf == NULL)
    {
       Error(routine,"Parametric surface not found");
       free(store_b);
       return NULL;
    }

    tDE = find_entry((long)store_b[2],1);
    if (tDE == NULL){
       Error(routine,"Entry not found");
       free(store_b);
       return NULL;
    }
    surf = (NurbSurf *)tDE->ndata;
    if (surf == NULL)
    {
       Error(routine,"Parent surface not found");
       free(store_b);
       return NULL;
    }

    psurf->parent_surf_ptr = surf;

    if (Delete == 1)
    {
       tDE->process = 1;
       tDE->visible = 2;
    }

    free(store_b);

    return (psurf);
}
