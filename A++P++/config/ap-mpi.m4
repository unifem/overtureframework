
dnl ********************************************************************
dnl * AXXPXX_PROG_MPICC searches the PATH for an available MPI C compiler
dnl * wraparound.  It assigns the name to MPICC.
dnl ********************************************************************

AC_DEFUN(AXXPXX_PROG_MPICC,
[
   AC_CHECK_PROGS(MPICC, mpcc_r mpcc mpicc tmcc hcc)
   test -z "$MPICC" && AC_MSG_ERROR([no acceptable mpicc found in \$PATH])
])dnl


dnl ********************************************************************
dnl * AXXPXX_PROG_MPICXX searches the PATH for an available MPI C++ 
dnl * compiler wraparound.  It assigns the name to MPICXX.
dnl ********************************************************************

AC_DEFUN(AXXPXX_PROG_MPICXX,
[
   AC_CHECK_PROGS(MPICXX, mpKCC_r mpKCC mpCC mpig++ mpiCC hcp)
   test -z "$MPICXX" && AC_MSG_ERROR([no acceptable mpic++ found in \$PATH])
])dnl


dnl **********************************************************************
dnl * AXXPXX_PROG_MPIF77 searches the PATH for an available MPI Fortran 77 
dnl * compiler wraparound.  It assigns the name to MPIF77.
dnl **********************************************************************

AC_DEFUN(AXXPXX_PROG_MPIF77,
[
   AC_CHECK_PROGS(MPIF77, mpf77 mpxlf mpif77 mpixlf tmf77 hf77)
   test -z "$MPIF77" && AC_MSG_ERROR([no acceptable mpif77 found in \$PATH])
])dnl


dnl ***********************************************************************
dnl * AXXPXX_CHECK_MPIF77_PP checks whether the preprocessor needs to
dnl * be called before calling the compiler for Fortran files with
dnl * preprocessor directives and MPI function calls.  If the preprocessor
dnl * is necessary, MPIF77NEEDSPP is set to "yes", otherwise it is set to
dnl * "no"
dnl ***********************************************************************

AC_DEFUN(AXXPXX_CHECK_MPIF77_PP,
[
   AC_REQUIRE([AXXPXX_PROG_MPIF77])

   rm -f testppmp.o

   AC_MSG_CHECKING(whether $FPP needs to be called before $MPIF77)

   # This follows the same procedur as AXXPXX_CHECK_F77_PP, except it tests
   # $MPIF77 using a test program that includes MPI functions.

   cat > testppmp.F <<EOF
#define FOO 3
	program testppmp
	include 'mpif.h'
	integer rank,size,mpierr,sum
	call MPI_INIT(mpierr)
	call MPI_COMM_SIZE(MPI_COMM_WORLD,size,mpierr)
	call MPI_COMM_RANK(MPI_COMM_WORLD,rank,mpierr)
#ifdef FORTRAN_NO_UNDERSCORE
        sum = rank + size
#else
        sum = rank + rank
#endif
        call MPI_FINALIZE(mpierr)
        end 
EOF

   $MPIF77 -DBAR -c testppmp.F 
   if test -f testppmp.o; then 
      MPIF77NEEDSPP=no 
   else 
      MPIF77NEEDSPP=yes 
   fi

   echo $MPIF77NEEDSPP
   rm -f testppmp.o testppmp.F
   AC_SUBST(MPIF77NEEDSPP)
])dnl


dnl *********************************************************************
dnl * AXXPXX_SET_MPI sets up the needed MPI library and directory flags.   
dnl * The location of the file mpi.h is put into the variable MPIINCLUDE
dnl * as a -I flag.  The -l flags that specify the needed libraries and
dnl * the -L flags that specify the paths of those libraries are placed in
dnl * the variables MPILIBS and MPILIBDIRS, respectively.  To set the MPI
dnl * libraries and directories manually, use the --with-mpi-include,
dnl * --with-mpi-libs, and --with-mpi-lib-dirs command-line options when
dnl * invoking configure.  Only one directory should be specified with
dnl * --with-mpi-include, while any number of directories can be specified
dnl * by --with-mpi-lib-dirs.  Any number of libraries can be specified
dnl * with --with-mpi-libs, and the libraries must be referred to by their 
dnl * base names, so libmpi.a is just mpi.  It is adviseable to use all 
dnl * three --with flags whenever one is used, because it is likely that
dnl * when one is chosen it will mess up the automatic choices for the
dnl * other two.  If the architecture is unknown, or if the needed MPI
dnl * settings for the current architecture are not known, then the naive
dnl * settings of MPILIBS="-lmpi" and MPILIBDIRS="-L/usr/local/mpi/lib"
dnl * are tested, and if they exist they are used, otherwise the MPILIB*
dnl * variables are left blank.  In the case of rs6000, the variable
dnl * MPIFLAGS is also set. 
dnl **********************************************************************

AC_DEFUN(AXXPXX_SET_MPI,
[
   AC_ARG_WITH(mpi-include, [  --with-mpi-include=DIR     DIR is a space-seperated list of include paths for MPI e.g. \"-I/usr/include -I/usr/local/include\"] , [axxpxx_mpi_include_dir=$withval])

   AC_ARG_WITH(mpi-libs, [  --with-mpi-libs=LIBS       LIBS is space-separated list of library names needed for MPI, e.g. \"-lnsl -lsocket -lmpi\"],[axxpxx_mpi_libs=$withval])

   AC_ARG_WITH(mpi-lib-dirs, [  --with-mpi-lib-dirs=DIRS      DIRS is space-separated list of directories containing the libraries specified by \`--with-mpi-libs', e.g \"-L/usr/lib -L/usr/local/mpi/lib\"], [axxpxx_mpi_lib_dirs=$withval])

   AC_ARG_WITH(mpirun, [  --with-mpirun=ARG         ARG is the absolute path of the executable used to run MPI programs.],[mpirun=$withval])

   if test -z "$axxpxx_mpi_libs"; then

      dnl This test replaces axxpxx-GUESS-ARCH
      if test -z "$host_os"; then
        AC_MSG_ERROR([host_os has not been defined in macro axxpxx-SET-MPI])
      fi

      dnl * Set everything to known values
      echo "In AXXPXX SET MPI host_os = $host_os"
      case $host_os in

         sun* | solaris*)
            if test -z "$axxpxx_mpi_include_dir"; then
               axxpxx_mpi_include_dir="-I/usr/local/mpi/mpich-1.1.1/include -I/usr/local/mpi/mpich-1.1.1/lib/solaris/ch_p4"
            fi

            if test -z "$axxpxx_mpi_lib_dirs"; then
               axxpxx_mpi_lib_dirs="-L/usr/local/mpi/mpich-1.1.1/lib/solaris/ch_p4"
            fi
            
            if test -z "$mpirun"; then
               mpirun=/usr/local/mpi/mpich-1.1.1/bin/mpirun
            fi 

            dnl axxpxx_mpi_libs="-lnsl -lsocket -lmpi";;
            axxpxx_mpi_libs="-lpmpich -lmpich -lsocket -lnsl -lpthread -lnsl -laio -lc";;

         osf*)
            if test -z "$axxpxx_mpi_include_dir"; then
               axxpxx_mpi_include_dir=-I/usr/opt/MPI190/include
            fi

            if test -z "$axxpxx_mpi_lib_dirs"; then
               axxpxx_mpi_lib_dirs="-L/usr/opt/MPI190/lib"
            fi

            if test -z "$mpirun"; then
               mpirun=/usr/opt/MPI190/bin/dmpirun
            fi 

            axxpxx_mpi_libs="-lmpi";;

         aix*) 
            if test -z "$axxpxx_mpi_include_dir"; then
               axxpxx_mpi_include_dir=-I/usr/lpp/ppe.poe/include
            fi

            if test -z "$axxpxx_mpi_lib_dirs"; then
               axxpxx_mpi_lib_dirs=-L/usr/lpp/ppe.poe/lib
            fi

            if test -z "$mpirun"; then
               mpirun=/usr/lpp/ppe.poe/bin/poe
            fi 

            axxpxx_mpi_libs=-lmpi

            MPIFLAGS="-binitfini:poe_remote_main";;

         irix*)
            if test -z "$axxpxx_mpi_include_dir"; then
             # axxpxx_mpi_include_dir="-I/usr/local/mpich-1.1.2/include -I/usr/local/mpich-1.1.2/build/IRIX64/ch_shmem/include"
               axxpxx_mpi_include_dir=
            fi

            if test -z "$axxpxx_mpi_lib_dirs"; then
             # axxpxx_mpi_lib_dirs=-L/usr/local/mpich-1.1.2/lib/IRIX64/ch_p4
             # axxpxx_mpi_lib_dirs=-L/usr/local/mpich-1.1.2/build/IRIX64/ch_shmem/lib
             # axxpxx_mpi_lib_dirs=-L/usr/lib64
               axxpxx_mpi_lib_dirs=
            fi

            if test -z "$mpirun"; then
               mpirun=/usr/bin/mpirun
             # mpirun=/usr/local/mpich-1.1.2/bin/mpirun
            fi 

            axxpxx_mpi_libs=-lmpi;; 
          # axxpxx_mpi_libs=-lmpich;;

       # Added an entry for linux machines
         linux*)
            if test -z "$axxpxx_mpi_include_dir"; then
               axxpxx_mpi_include_dir=-I/usr/casc/overture/local-i686-redhat-linux/opt/mpich/include
            fi

            if test -z "$axxpxx_mpi_lib_dirs"; then
               axxpxx_mpi_lib_dirs=-L/usr/casc/overture/local-i686-redhat-linux/opt/mpich/lib
            fi

            if test -z "$mpirun"; then
               mpirun=/usr/casc/overture/local-i686-redhat-linux/opt/mpich/bin/mpirun
            fi 

            axxpxx_mpi_libs=-lmpich;; 
        
         *)
            AC_MSG_WARN([trying naive MPI settings - can use --with flags to change])
            if test -z "$axxpxx_mpi_include_dir"; then
               axxpxx_mpi_include_dir=-I/usr/local/mpi/include
            fi

            if test -z "$axxpxx_mpi_lib_dirs"; then
               axxpxx_mpi_lib_dirs=-L/usr/local/mpi/lib
            fi
            axxpxx_mpi_libs=-lmpi ;;
      esac

    fi

    if test -n "$axxpxx_mpi_include_dir"; then 
       MPIINCLUDE="$axxpxx_mpi_include_dir"
    else
       MPIINCLUDE=
    fi

  # This can just be set without a loop (which should be an error if there is more than one lib dir in the list!)
    if test -n "$axxpxx_mpi_lib_dirs"; then
       MPILIBDIRS="$axxpxx_mpi_lib_dirs"
    else
       MPILIBDIRS=
    fi

    for axxpxx_lib in $axxpxx_mpi_libs; do
       MPILIBS="$MPILIBS $axxpxx_lib"
    done

    echo "In AXXPXX SET MPI: MPIINCLUDE = $MPIINCLUDE"
    echo "In AXXPXX SET MPI: MPILIBDIRS = $MPILIBDIRS"
    echo "In AXXPXX SET MPI: MPILIBS    = $MPILIBS"
    echo "In AXXPXX SET MPI: MPIFLAGS   = $MPIFLAGS"
    echo "In AXXPXX SET MPI: mpirun     = $mpirun"

   AC_SUBST(MPIINCLUDE)
   AC_SUBST(MPILIBDIRS)
   AC_SUBST(MPILIBS)
   AC_SUBST(MPIFLAGS)
   AC_SUBST(mpirun)
])


# This is an older version of the AXXPXX SET MPI macro (it can be removed)
AC_DEFUN(OLD_AXXPXX_SET_MPI,
[
   AC_ARG_WITH(mpi-include, [  --with-mpi-include=DIR     DIR is a
space-seperated list of include paths for MPI e.g. \"-I/usr/include
-I/usr/local/include\"] , [axxpxx_mpi_include_dir=$withval])

   AC_ARG_WITH(mpi-libs, [  --with-mpi-libs=LIBS       LIBS is space-separated list of library names needed for MPI, e.g. \"-lnsl -lsocket -lmpi\"],[axxpxx_mpi_libs=$withval])

   AC_ARG_WITH(mpi-lib-dirs, [  --with-mpi-lib-dirs=DIRS      DIRS is space-separated list of directories containing the libraries specified by \`--with-mpi-libs', e.g \"-L/usr/lib -L/usr/local/mpi/lib\"], [axxpxx_mpi_lib_dirs=$withval])


   if test -z "$axxpxx_mpi_libs"; then

      dnl This test replaces axxpxx-GUESS-ARCH
      if test -z "$host_os"; then
        AC_MSG_ERROR([host_os has not been defined in macro axxpxx-SET-MPI])
      fi

      dnl * Set everything to known values
      echo "In AXXPXX SET MPI host_os = $host_os"
      case $host_os in

         sun* | solaris*)
            if test -z "$axxpxx_mpi_include_dir"; then
               dnl axxpxx_mpi_include_dir="-I/usr/local/mpi/mpich/include -I/usr/local/mpi/mpich/lib/solaris/ch_p4"
               axxpxx_mpi_include_dir="-I/usr/local/mpi/mpich-1.1.1/include -I/usr/local/mpi/mpich-1.1.1/lib/solaris/ch_p4"
            fi

            if test -z "$axxpxx_mpi_lib_dirs"; then
               dnl axxpxx_mpi_lib_dirs="-L/usr/local/mpi/mpich/lib/solaris/ch_p4 -L/usr/lib"
               axxpxx_mpi_lib_dirs="-L/usr/local/mpi/mpich-1.1.1/lib/solaris/ch_p4"
            fi
            
            dnl axxpxx_mpi_libs="-lnsl -lsocket -lmpi";;
            axxpxx_mpi_libs="-lpmpich -lmpich -lsocket -lnsl -lpthread -lnsl -laio -lc";;

         osf*)
            if test -z "$axxpxx_mpi_include_dir"; then
               axxpxx_mpi_include_dir=-I/usr/opt/MPI190/include
            fi

            if test -z "$axxpxx_mpi_lib_dirs"; then
               axxpxx_mpi_lib_dirs="-L/usr/opt/MPI190/lib"
            fi

            axxpxx_mpi_libs="-lmpi";;

         aix*) 
            if test -z "$axxpxx_mpi_include_dir"; then
               axxpxx_mpi_include_dir=-I/usr/lpp/ppe.poe/include
            fi

            if test -z "$axxpxx_mpi_lib_dirs"; then
               axxpxx_mpi_lib_dirs=-L/usr/lpp/ppe.poe/lib
            fi

            axxpxx_mpi_libs=-lmpi

            MPIFLAGS="-binitfini:poe_remote_main";;

         irix*)
            if test -z "$axxpxx_mpi_include_dir"; then
               axxpxx_mpi_include_dir=-I/usr/local/mpi/include
            fi

            if test -z "$axxpxx_mpi_lib_dirs"; then
               axxpxx_mpi_lib_dirs=-L/usr/local/mpi/lib/IRIX64/ch_p4
            fi

            axxpxx_mpi_libs=-lmpi;; 

       # Added an entry for linux machines
         linux*)
            if test -z "$axxpxx_mpi_include_dir"; then
               axxpxx_mpi_include_dir=-I/usr/casc/overture/local-i686-redhat-linux/opt/mpich/include
            fi

            if test -z "$axxpxx_mpi_lib_dirs"; then
               axxpxx_mpi_lib_dirs=/usr/casc/overture/local-i686-redhat-linux/opt/mpich/lib
            fi

            axxpxx_mpi_libs=-lmpich;; 
        
         *)
AC_MSG_WARN([trying naive MPI settings - can use --with flags to change])
            if test -z "$axxpxx_mpi_include_dir"; then
               axxpxx_mpi_include_dir=-I/usr/local/mpi/include
            fi

            if test -z "$axxpxx_mpi_lib_dirs"; then
               axxpxx_mpi_lib_dirs=-L/usr/local/mpi/lib
            fi
            axxpxx_mpi_libs=-lmpi ;;
      esac

    fi

    if test -n "$axxpxx_mpi_include_dir"; then 
       MPIINCLUDE="$axxpxx_mpi_include_dir"
    else
       MPIINCLUDE=
    fi

  # This can just be set without a loop (which should be an error if there is more than one lib dir in the list!)
    if test -n "$axxpxx_mpi_lib_dirs"; then
       MPILIBDIRS="$axxpxx_mpi_lib_dirs"
       # for axxpxx_lib_dir in $axxpxx_mpi_lib_dirs; do
       #    MPILIBDIRS="$axxpxx_lib_dir $MPILIBDIRS"
       # done
    else
       MPILIBDIRS=
    fi

    for axxpxx_lib in $axxpxx_mpi_libs; do
       MPILIBS="$MPILIBS $axxpxx_lib"
    done

    echo "In AXXPXX SET MPI: MPIINCLUDE = $MPIINCLUDE"
    echo "In AXXPXX SET MPI: MPILIBDIRS = $MPILIBDIRS"
    echo "In AXXPXX SET MPI: MPILIBS    = $MPILIBS"
])



dnl ********************************************************************
dnl * AXXPXX_FIND_MPI will determine the libraries, directories, and other
dnl * flags needed to compile and link programs with MPI function calls.
dnl * This macro runs tests on the script found by the AXXPXX_PROG_MPICC
dnl * macro.  If there is no such mpicc-type script in the PATH and
dnl * MPICC is not set manually, then this macro will not work.
dnl *
dnl * One may question why these settings would need to be determined if
dnl * there already is mpicc available, and that is a valid question.  I
dnl * can think of a couple of reasons one may want to use these settings 
dnl * rather than using mpicc directly.  First, these settings allow you
dnl * to choose the C compiler you wish to use rather than using whatever
dnl * compiler is written into mpicc.  Also, the settings determined by
dnl * this macro should also work with C++ and Fortran compilers, so you
dnl * won't need to have mpiCC and mpif77 alongside mpicc.  This is
dnl * especially helpful on systems that don't have mpiCC.  The advantage
dnl * of this macro over AXXPXX_SET_MPI is that this one doesn't require
dnl * a test of the machine type and thus will hopefully work on unknown
dnl * architectures.  The main disadvantage is that it relies on mpicc.
dnl *
dnl * --with-mpi-include, --with-mpi-libs, and --with-mpi-lib-dirs can be
dnl * used to manually override the automatic test, just as with
dnl * AXXPXX_SET_MPI.  If any one of these three options are used, the
dnl * automatic test will not be run, so it is best to call all three
dnl * whenever one is called.  In addition, the option --with-mpi-flags is
dnl * available here to set any other flags that may be needed, but it
dnl * does not override the automatic test.  Flags set by --with-mpi-flags
dnl * will be added to the variable MPIFLAGS.  This way, if the macro, for
dnl * whatever reason, leaves off a necessary flag, the flag can be added 
dnl * to MPIFLAGS without eliminating anything else.  The other variables
dnl * set are MPIINCLUDE, MPILIBS, and MPILIBDIRS, just as in 
dnl * AXXPXX_SET_MPI.  This macro also incorporates AXXPXX_SET_MPI as a backup
dnl * plan, where if there is no mpicc, it will use the settings
dnl * determined by architecture name in AXXPXX_SET_MPI
dnl ********************************************************************

AC_DEFUN(AXXPXX_FIND_MPI,
[
   axxpxx_find_mpi_cache_used=yes

   AC_MSG_CHECKING([for MPI])
   AC_CACHE_VAL(axxpxx_cv_mpi_include, axxpxx_find_mpi_cache_used=no)
   AC_CACHE_VAL(axxpxx_cv_mpi_libs, axxpxx_find_mpi_cache_used=no)
   AC_CACHE_VAL(axxpxx_cv_mpi_lib_dirs, axxpxx_find_mpi_cache_used=no)
   AC_CACHE_VAL(axxpxx_cv_mpi_flags, axxpxx_find_mpi_cache_used=no)
   AC_MSG_RESULT( )

   if test "$axxpxx_find_mpi_cache_used" = "yes"; then

      echo "TEST A: Case of cached values"

      AC_MSG_CHECKING([for location of mpi.h])
      MPIINCLUDE=$axxpxx_cv_mpi_include
      AC_MSG_RESULT([\(cached\) $MPIINCLUDE])

      AC_MSG_CHECKING(for MPI library directories)
      MPILIBDIRS=$axxpxx_cv_mpi_lib_dirs
      AC_MSG_RESULT([\(cached\) $MPILIBDIRS])

      AC_MSG_CHECKING(for MPI libraries)
      MPILIBS=$axxpxx_cv_mpi_libs
      AC_MSG_RESULT([\(cached\) $MPILIBS])

      AC_MSG_CHECKING(for other MPI-related flags)
      MPIFLAGS=$axxpxx_cv_mpi_flags
      AC_MSG_RESULT([\(cached\) $MPIFLAGS])
   else
   
      echo "TEST B: Case of NON cached values"


      dnl * Set up user options.  If user uses any of the fist three options,
      dnl * then automatic tests are not run.

      axxpxx_user_chose_mpi=no
      AC_ARG_WITH(mpi-include, [  --with-mpi-include=DIR  mpi.h is in DIR],
                  for mpi_dir in $withval; do
                     MPIINCLUDE="$MPIINCLUDE -I$withval"
                  done; axxpxx_user_chose_mpi=yes)

      AC_ARG_WITH(mpi-libs,
[  --with-mpi-libs=LIBS    LIBS is space-separated list of library names 
                          needed for MPI, e.g. \"nsl socket mpi\"],  
                  for mpi_lib in $withval; do
                     MPILIBS="$MPILIBS -l$mpi_lib"
                  done; axxpxx_user_chose_mpi=yes)


      AC_ARG_WITH(mpi-lib-dirs,
[  --with-mpi-lib-dirs=DIRS
                          DIRS is space-separated list of directories
                          containing the libraries specified by
                          \`--with-mpi-libs', e.g \"/usr/lib /usr/local/mpi/lib\"],
                  for mpi_lib_dir in $withval; do
                     MPILIBDIRS="-L$mpi_lib_dir $MPILIBDIRS"
                  done; axxpxx_user_chose_mpi=yes)

      dnl * --with-mpi-flags only adds to automatic selections, 
      dnl * does not override

      AC_ARG_WITH(mpi-flags,
[  --with-mpi-flags=FLAGS  FLAGS is space-separated list of whatever flags other
                          than -l and -L are needed to link with mpi libraries],
                          MPIFLAGS=$withval)


      if test "$axxpxx_user_chose_mpi" = "no"; then

      dnl * Find an MPICC.  If there is none, call AXXPXX_SET_MPI to choose MPI
      dnl * settings based on architecture name.  If AXXPXX_SET_MPI fails,
      dnl * print warning message.  Manual MPI settings must be used.

         AC_ARG_WITH(MPICC,
[  --with-MPICC=ARG        ARG is mpicc or similar MPI C compiling tool],
            MPICC=$withval,
            [AC_CHECK_PROGS(MPICC, mpcc_r mpcc mpicc tmcc hcc)])

         if test -z "$MPICC"; then
            AC_MSG_WARN([no acceptable mpicc found in \$PATH])
            ##AXXPXX_SET_MPI
            if test -z "$MPILIBS"; then
             AC_MSG_WARN([MPI not found - must set manually using --with flags])
            fi

         dnl * When $MPICC is there, run the automatic test
         dnl * here begins the hairy stuff

         else      
 
dnl            changequote(, )dnl
  
            AC_MSG_CHECKING([for location of mpi.h])

            dnl * Create a minimal MPI program.  It will be compiled using
            dnl * $MPICC with verbose output.
            cat > mpconftest.c << EOF
#include "mpi.h"

main(int argc, char **argv)
{
   int rank, size;
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Finalize();
   return 0;
}
EOF

            axxpxx_mplibs=
            axxpxx_mplibdirs=
            axxpxx_flags=
            axxpxx_lmpi_exists=no

            dnl * These are various ways to produce verbose output from $MPICC
            dnl * All of their outputs are stuffed into variable
            dnl * $axxpxx_mpoutput

            for axxpxx_command in "$MPICC -show"\
                                "$MPICC -v"\
                                "$MPICC -#"\
                                "$MPICC"; do

               axxpxx_this_output=`$axxpxx_command mpconftest.c -o mpconftest 2>&1`

               dnl * If $MPICC uses xlc, then commas must be removed from output
               xlc_p=`echo $axxpxx_this_output | grep xlcentry`
               if test -n "$xlc_p"; then
                  axxpxx_this_output=`echo $axxpxx_this_output | sed 's/,/ /g'`
               fi

               dnl * Turn on flag once -lmpi is found in output
               lmpi_p=`echo $axxpxx_this_output | grep "\-lmpi"`
               if test -n "$lmpi_p"; then
                  axxpxx_lmpi_exists=yes
               fi

               axxpxx_mpoutput="$axxpxx_mpoutput $axxpxx_this_output"
               axxpxx_this_output=

            done

            rm -rf mpconftest*

            dnl * little test to identify $CC as IBM's xlc
            echo "main() {}" > cc_conftest.c
            cc_output=`${CC-cc} -v -o cc_conftest cc_conftest.c 2>&1`
            xlc_p=`echo $cc_output | grep xlcentry`
            if test -n "$xlc_p"; then
               axxpxx_compiler_is_xlc=yes
            fi 
            rm -rf cc_conftest*

            dnl * $MPICC might not produce '-lmpi', but we still need it.
            dnl * Add -lmpi to $axxpxx_mplibs if it was never found
            if test "$axxpxx_lmpi_exists" = "no"; then
               axxpxx_mplibs="-lmpi"
            else
               axxpxx_mplibs=
            fi

            axxpxx_want_arg=

            dnl * Loop through every word in output to find possible flags.
            dnl * If the word is the absolute path of a library, it is added
            dnl * to $axxpxx_flags.  Any "-llib", "-L/dir", "-R/dir" and
            dnl * "-I/dir" is kept.  If '-l', '-L', '-R', '-I', '-u', or '-Y'
            dnl * appears alone, then the next word is checked.  If the next
            dnl * word is another flag beginning with '-', then the first
            dnl * word is discarded.  If the next word is anything else, then
            dnl * the two words are coupled in the $axxpxx_arg variable.
            dnl * "-binitfini:poe_remote_main" is a flag needed especially
            dnl * for IBM MPI, and it is always kept if it is found.
            dnl * Any other word is discarded.  Also, after a word is found
            dnl * and kept once, it is discarded if it appears again

            echo "TEST C: $axxpxx_mpoutput"

            for axxpxx_arg in $axxpxx_mpoutput; do

               echo "Top of loop"
               echo "axxpxx_arg = $axxpxx_arg"

               axxpxx_old_want_arg=$axxpxx_want_arg
               axxpxx_want_arg=  

               if test -n "$axxpxx_old_want_arg"; then
                  case "$axxpxx_arg" in
                  -*)
                     axxpxx_old_want_arg=
                  ;;
                  esac
               fi

               case "$axxpxx_old_want_arg" in
               '')
                  case $axxpxx_arg in
                  /*.a)
                     exists=false
                     for f in $axxpxx_flags; do
                        if test x$axxpxx_arg = x$f; then
                           exists=true
                        fi
                     done
                     if $exists; then
                        axxpxx_arg=
                     else
                        axxpxx_flags="$axxpxx_flags $axxpxx_arg"
                     fi
                  ;;
                  -binitfini:poe_remote_main)
                     exists=false
                     for f in $axxpxx_flags; do
                        if test x$axxpxx_arg = x$f; then
                           exists=true
                        fi
                     done
                     if $exists; then
                        axxpxx_arg=
                     else
                        axxpxx_flags="$axxpxx_flags $axxpxx_arg"
                     fi
                  ;;
                  -lang*)
                     axxpxx_arg=
                  ;;
                  -[lLR])
                     axxpxx_want_arg=$axxpxx_arg
                     axxpxx_arg=
                  ;;
                  -[lLR]*)
                     exists=false
                     for f in $axxpxx_flags; do
                        if test x$axxpxx_arg = x$f; then
                           exists=true
                        fi
                     done
                     if $exists; then
                        axxpxx_arg=
                     else
                       axxpxx_flags="$axxpxx_flags $axxpxx_arg"
                     fi
                  ;;
                  -u)
                     axxpxx_want_arg=$axxpxx_arg
                     axxpxx_arg=
                  ;;
                  -Y)
                     axxpxx_want_arg=$axxpxx_arg
                     axxpxx_arg=
                  ;;
                  -I)
                     axxpxx_want_arg=$axxpxx_arg
                     axxpxx_arg=
                  ;;
                  -I*)
                     exists=false
                     for f in $axxpxx_flags; do
                        if test x$axxpxx_arg = x$f; then
                           exists=true
                        fi
                     done
                     if $exists; then
                        axxpxx_arg=
                     else
                        axxpxx_flags="$axxpxx_flags $axxpxx_arg"
                     fi
                  ;;
                  *)
                     axxpxx_arg=
                  ;;
                  esac

               ;;
               -[lLRI])
                  axxpxx_arg="axxpxx_old_want_arg $axxpxx_arg"
               ;;
               -u)
                  axxpxx_arg="-u $axxpxx_arg"
               ;;
               -Y)
                  axxpxx_arg=`echo $axxpxx_arg | sed -e 's%^P,%%'`
                  SAVE_IFS=$IFS
                  IFS=:
                  axxpxx_list=
                  for axxpxx_elt in $axxpxx_arg; do
                     axxpxx_list="$axxpxx_list -L$axxpxx_elt"
                  done
                  IFS=$SAVE_IFS
                  axxpxx_arg="$axxpxx_list"
               ;;
               esac

               dnl * Still inside the big for loop, we separate each flag
               dnl * into includes, libdirs, libs, flags
               if test -n "$axxpxx_arg"; then
                  case $axxpxx_arg in
                  -I*)

                     dnl * if the directory given in this flag contains mpi.h
                     dnl * then the flag is assigned to $MPIINCLUDE
                     if test -z "$MPIINCLUDE"; then
                        axxpxx_cppflags="$axxpxx_cppflags $axxpxx_arg"
                        axxpxx_include_dir=`echo "$axxpxx_arg" | sed 's/-I//g'` 

                        SAVE_CPPFLAGS="$CPPFLAGS"
                        CPPFLAGS="$axxpxx_cppflags"
dnl                        changequote([, ])dnl

                        unset ac_cv_header_mpi_h
                        AC_CHECK_HEADER(mpi.h,
                                        MPIINCLUDE="$axxpxx_cppflags")

dnl                        changequote(, )dnl
                        CPPFLAGS="$SAVE_CPPFLAGS"

                     else
                        axxpxx_arg=
                     fi
                  ;;
                  -[LR]*)

                     dnl * These are the lib directory flags
                     axxpxx_mplibdirs="$axxpxx_mplibdirs $axxpxx_arg"
                  ;;
                  -l* | /*)

                     dnl * These are the libraries
                     axxpxx_mplibs="$axxpxx_mplibs $axxpxx_arg"
                  ;;
                  -binitfini:poe_remote_main)
                     if test "$axxpxx_compiler_is_xlc" = "yes"; then
                        axxpxx_mpflags="$axxpxx_mpflags $axxpxx_arg"
                     fi
                  ;;
                  *)
                     dnl * any other flag that has been kept goes here
                     axxpxx_mpflags="$axxpxx_mpflags $axxpxx_arg"
                  ;;
                  esac

                  dnl * Upcoming test needs $LIBS to contain the flags 
                  dnl * we've found
                  LIBS_SAVE=$LIBS
                  LIBS="$MPIINCLUDE $axxpxx_mpflags $axxpxx_mplibdirs $axxpxx_mplibs"

                  if test -n "`echo $LIBS | grep '\-R/'`"; then
                     LIBS=`echo $LIBS | sed 's/-R\//-R \//'`
                  fi

dnl                  changequote([, ])dnl


                  dnl * Test to see if flags found up to this point are
                  dnl * sufficient to compile and link test program.  If not,
                  dnl * the loop keeps going to the next word
                  AC_TRY_LINK(
dnl [#ifdef __cplusplus
dnl extern "C"
dnl #endif
dnl ]
[#include "mpi.h"
], [int rank, size;
   int argc;
   char **argv;
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Finalize();
],
                     axxpxx_result=yes)

                  LIBS=$LIBS_SAVE

                  if test "$axxpxx_result" = yes; then
                     axxpxx_result=
                     break
                  fi
               fi

               echo "Bottom of loop"
               echo "axxpxx_flags = $axxpxx_flags"
               echo "axxpxx_mpflags = $axxpxx_mpflags"
               echo "axxpxx_mplibs = $axxpxx_mplibs"
               echo "axxpxx_mplibdirs = $axxpxx_mplibdirs"

            done

            echo "TEST D: After loop is done, set variables to be substituted"
            dnl * After loop is done, set variables to be substituted
            MPILIBS=$axxpxx_mplibs
            MPILIBDIRS=$axxpxx_mplibdirs
            MPIFLAGS="$MPIFLAGS $axxpxx_mpflags"

            dnl * IBM MPI uses /usr/lpp/ppe.poe/libc.a instead of /lib/libc.a
            dnl * so we need to make sure that -L/lib is not part of the 
            dnl * linking line when we use IBM MPI.  This only appears in
            dnl * configure when AXXPXX_FIND_MPI is called first.
            ifdef([AC_PROVIDE_AXXPXX_FIND_F77LIBS], 
               if test -n "`echo $F77LIBFLAGS | grep '\-L/lib '`"; then
                  if test -n "`echo $F77LIBFLAGS | grep xlf`"; then
                     F77LIBFLAGS=`echo $F77LIBFLAGS | sed 's/-L\/lib //g'`
                  fi
               fi
            )

            echo "TEST E:"

            if test -n "`echo $MPILIBS | grep pmpich`" &&
               test -z "`echo $MPILIBS | grep pthread`"; then
                  LIBS_SAVE=$LIBS
                  LIBS="$MPIINCLUDE $MPIFLAGS $MPILIBDIRS $MPILIBS -lpthread"
                  AC_TRY_LINK(
dnl                      ifelse(AC_LANG, CPLUSPLUS,
dnl [#ifdef __cplusplus
dnl extern "C"
dnl #endif
dnl ])dnl
[#include "mpi.h"
], [int rank, size;
   int argc;
   char **argv;
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Finalize();
],
                     MPILIBS="$MPILIBS -lpthread")
                  LIBS=$LIBS_SAVE
            fi
          

            echo "TEST F:"

            AC_MSG_RESULT($MPIINCLUDE)
            AC_MSG_CHECKING([for MPI library directories])
            AC_MSG_RESULT($MPILIBDIRS)
            AC_MSG_CHECKING([for MPI libraries])
            AC_MSG_RESULT($MPILIBS)
            AC_MSG_CHECKING([for other MPI-related flags])
            AC_MSG_RESULT($MPIFLAGS)
         fi
      fi

      AC_CACHE_VAL(axxpxx_cv_mpi_include, axxpxx_cv_mpi_include=$MPIINCLUDE)
      AC_CACHE_VAL(axxpxx_cv_mpi_lib_dirs, axxpxx_cv_mpi_lib_dirs=$MPILIBDIRS)
      AC_CACHE_VAL(axxpxx_cv_mpi_libs, axxpxx_cv_mpi_libs=$MPILIBS)
      AC_CACHE_VAL(axxpxx_cv_mpi_flags, axxpxx_cv_mpi_flags=$MPIFLAGS)
   fi

   AC_SUBST(MPIINCLUDE)
   AC_SUBST(MPILIBDIRS)
   AC_SUBST(MPILIBS)
   AC_SUBST(MPIFLAGS)

])dnl
