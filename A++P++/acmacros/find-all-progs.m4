dnl $Id: find-all-progs.m4,v 1.2 2001/10/22 18:39:23 gunney Exp $

dnl BTNG_PATH_FINDALLPROGS documentation.
dnl BTNG_PATH_FINDALLPROGS( program-to-check-for[, path] )
dnl Finds all instances of program-to-check-for in the path.
dnl Sets the variable program-to-check-for_paths (with all '-'
dnl changed to '_') to a space-separated list of paths to the
dnl instances found.
AC_DEFUN(BTNG_PATH_FINDALLPROGS,[
  space_separated_paths=`echo $2 | sed 's/:/ /g'`
  prog_name=`echo $1 | sed 's/-/_/g'`
  for dir in $space_separated_paths
  do
    if test -x ${dir}/$prog_name; then
      eval "${prog_name}_paths=\"\$${prog_name}_paths ${dir}/$1\""
    fi
  done
])	dnl End definition of macro BTNG_PATH_FINDALLPROGS.
