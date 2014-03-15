/*  FORTRAN calling convention definition.

    Define the convention for FORTRAN/C and C/FORTRAN calls.

       This definition works on the following machines (amazing!)
       (the symbol identified with the machine is defined automatically
       by the system):

          Symbol                Machine
          ------                -------
          CRAY1                 CRAY X-MP, Y-MP, C-90/Unicos
          CRAY2                 CRAY 2/Unicos
          convex                Convex C-1 (and others?)
          hppa                  Hewlett Packard HP 9000 POSIX
          sgi && m68000         Silicon Graphics IRIS 3030 etc.
          sgi && mips           Silicon Graphics IRIS 4D series
          sun                   Sun 3/50 (and others?)
          UTS                   Amdahl/UTS
          -                     VAX/Unix 4.3 BSD
          -                     DEC Alpha Workstation

       For the following machine(s), the appropriate symbol must be defined
       (e.g., use "-Dstardent" in the cpp statement):

          Symbol                Machine
          ------                -------
          DEC_ALPHA             DEC Alpha
          rios                  IBM RS-6000
          stardent              Stardent (Titan?)


   Pieter G. Buning  3/25/92                                                 */

#ifndef __FORTCALL

#if CRAY1 || CRAY2 || stardent   /*  CRAY/Unicos or Stardent */
#define FORTCALL_UC              /*  Use uppercase routine name.  */
#define __FORTCALL
#endif

#if sgi && m68000                /*  SGI IRIS 2xxx/3xxx  */
#define FORTCALL_DECL_REVARG     /*  Declare routine to be type fortran and  */
#define __FORTCALL               /*  reverse arguments.  */
#endif

#if convex || sun || (sgi && mips) || UTS
                                 /*  Convex or Sun or IRIS 4D or Amdahl/UTS  */
#define FORTCALL_TR_US           /*  Use (lowercase) name with a trailing  */
#define __FORTCALL               /*  underscore.  */
#endif

#if rios || hppa                 /*  IBM RS-6000 or HP 9000  */
#define FORTCALL_LC              /*  Use (lowercase) name.  */
#define __FORTCALL
#endif

#ifndef __FORTCALL               /*  Default (seems most prevalent)  */
#define FORTCALL_TR_US           /*  Use (lowercase) name with a trailing  */
#define __FORTCALL               /*  underscore.  */
#endif

#endif

