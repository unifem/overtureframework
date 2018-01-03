dnl $Id: support-samrai.m4,v 1.22 2002/01/30 16:32:42 gunney Exp $


AC_DEFUN(BTNG_SUPPORT_SAMRAI,[
dnl Support SAMRAI by setting the variables samrai_INCLUDES and samrai_LIBS.
dnl User will specify where the SAMRAI source is located (--samrai-src=...)
dnl and where SAMRAI was compiled (--samrai_compile=...).
dnl In addition, user may omit certain "dimensional libraries" in SAMRAI
dnl by specifying  --samrai-exclude-dim=string, where string contains
dnl combinations of '1', '2' and '3', the directions to exclude.
dnl
dnl This macro also sets certain variables useful when including SAMRAI
dnl sources in Doxygen documentation.
dnl
dnl SAMRAI_EXCLUDE_DIM: dimensions to exclude (some conbination of 1, 2 and 3)

# Set SAMRAI_COMPILE to the SAMRAI compile directory.
BTNG_ARG_WITH_PREFIX(samrai-compile,SAMRAI_COMPILE,
[  --with-samrai-compile=PATH
			Specify where the SAMRAI compile directory is located.
			This should be a properly configured SAMRAI and the
			libraries you expect to use should already be built.])
if test ! -d "$SAMRAI_COMPILE"; then
  AC_MSG_WARN([SAMRAI compile directory ($SAMRAI_COMPILE) does not look right])
fi

if test -r "$SAMRAI_COMPILE/config.status"; then
  # See if we can guess the SAMRAI source directory from the compile directory.
  # Do not issue warnings, because we may not actually need to do this,
  # depending on whether the with-samrai-src option is issued.
  tmpstr=`sed -n -e '/^srcdir=/!D' -e 's/^srcdir=//' -e p -e q $SAMRAI_COMPILE/config.status 2>/dev/null`
  test "$tmpstr" && btng_guess_samrai_src=`cd $SAMRAI_COMPILE && cd $tmpstr && pwd`
  BTNG_AC_LOG_VAR(SAMRAI_COMPILE tmpstr btng_guess_samrai_src)
fi

# Set SAMRAI_SRC to the SAMRAI source directory.
BTNG_ARG_WITH_PREFIX(samrai-src,SAMRAI_SRC,
[  --with-samrai-src=PATH
			Specify where the SAMRAI source directory is located.
			If omitted, it can be guessed from the contents of
			of a properly configured SAMRAI compile directory.]
,[
# Set SAMRAI source directory to value guessed from the compile directory.
if test -n "$btng_guess_samrai_src"; then
  SAMRAI_SRC=$btng_guess_samrai_src
fi
])
if test ! -d "$SAMRAI_SRC"; then
  AC_MSG_WARN([
SAMRAI source directory ($SAMRAI_SRC) does not look right
You can set this directory using --with-samrai-src=...
])
fi

# To reduce clutter in the link flags, this is a way to remove
# certain dimensions from the SAMRAI library.
BTNG_ARG_WITH_ENV_WRAPPER(samrai-exclude-dim,SAMRAI_EXCLUDE_DIM,
[  --with-samrai-exclude-dim=STRING
			Specify list of dimensions (1,2,3) to exclude
			from SAMRAI library],,[
  # SAMRAI_EXCLUDE_DIM may only contain 1, 2 or 3.
  SAMRAI_EXCLUDE_DIM=`echo $SAMRAI_EXCLUDE_DIM | sed 's/[[[^123]]]//g'`
])

samrai_INCLUDES="-I$SAMRAI_COMPILE/include -I$SAMRAI_SRC/include"

# Determine the samrai library names.
samrai_libs_ls1=`cd $SAMRAI_COMPILE/lib && echo libSAMRAI*.*`
if test "$samrai_libs_ls1" = 'libSAMRAI*.*'; then
  samrai_libs_ls1='libSAMRAI.a libSAMRAI1d_algs.a libSAMRAI1d_geom.a libSAMRAI1d_hier.a libSAMRAI1d_math_special.a libSAMRAI1d_math_std.a libSAMRAI1d_mesh.a libSAMRAI1d_pdat_special.a libSAMRAI1d_pdat_std.a libSAMRAI1d_solv.a libSAMRAI1d_tbox.a libSAMRAI1d_xfer.a libSAMRAI2d_algs.a libSAMRAI2d_geom.a libSAMRAI2d_hier.a libSAMRAI2d_math_special.a libSAMRAI2d_math_std.a libSAMRAI2d_mesh.a libSAMRAI2d_pdat_special.a libSAMRAI2d_pdat_std.a libSAMRAI2d_plot.a libSAMRAI2d_solv.a libSAMRAI2d_tbox.a libSAMRAI2d_xfer.a libSAMRAI3d_algs.a libSAMRAI3d_geom.a libSAMRAI3d_hier.a libSAMRAI3d_math_special.a libSAMRAI3d_math_std.a libSAMRAI3d_mesh.a libSAMRAI3d_pdat_special.a libSAMRAI3d_pdat_std.a libSAMRAI3d_plot.a libSAMRAI3d_solv.a libSAMRAI3d_tbox.a libSAMRAI3d_xfer.a'
  AC_MSG_WARN(
[I cannot find any library files in $SAMRAI_COMPILE/lib.
I am resorting to a priori knowledge of SAMRAI libraries,
which MAY be outdated.  I am assuming the following libraries:
$samrai_libs_ls1])
fi
# Remove excluded dimensions, if any.
test -n "$SAMRAI_EXCLUDE_DIM" &&	\
  [samrai_libs_ls1="`echo $samrai_libs_ls1 | sed -e 's/libSAMRAI['$SAMRAI_EXCLUDE_DIM']d_[a-zA-Z0-9_]\{1,\}\.a//g' -e 's/libSAMRAI['$SAMRAI_EXCLUDE_DIM']d_[a-zA-Z0-9_]\{1,\}\.so//g'`"]
if test -n "$samrai_libs_ls1"; then
  unset samrai_libs_ls
  for i in $samrai_libs_ls1; do
    j=`echo $i | sed -e 's/lib//' -e 's/\.a$//' -e 's/\.so$//'`
    if echo "$samrai_libs_ls" | grep -v " $j " > /dev/null; then # Note padding!
      samrai_libs_ls="$samrai_libs_ls $j ";	# Note space padding!
    fi
  done
fi
# Move some low-level libraries to the end to ensure resolution
# for linkers that only make one pass.
for d in 3 2 1; do
  for i in plot geom solv algs mesh math_std math_special pdat_std pdat_special xfer hier tbox; do
    [samrai_libs_ls=`echo "$samrai_libs_ls" | sed 's/\(.*\)\( SAMRAI'$d'd_'$i' \)\(.*\)/\1 \3 \2/g'`]
  done
done
[samrai_libs_ls=`echo "$samrai_libs_ls" | sed 's/\(.*\)\( SAMRAI \)\(.*\)/\1 \3 \2/g'`]
dnl [samrai_libs_ls=`echo "$samrai_libs_ls" | sed 's/ \{2,\}/ /g'`]
# Build up SAMRAI_LIBS string using library names.
BTNG_AC_LOG_VAR(samrai_libs_ls1 samrai_libs_ls)
if test -n "$samrai_libs_ls"; then
  for i in $samrai_libs_ls; do
    samrai_LIBS="$samrai_LIBS -l$i"
  done
fi
samrai_LIBS="-L$SAMRAI_COMPILE/lib $samrai_LIBS"

BTNG_AC_LOG_VAR(SAMRAI_EXCLUDE_DIM samrai_INCLUDES samrai_LIBS)



# The following paragraph refers to generating Doxygen documentation for
# your source code and including SAMRAI source codes also.  If you do not
# know that this means, you can ignore it.
#
# Whether and how SAMRAI sources should be included in doxygen documentation.
# These autoconf variables are available for use in a doxygen configuration
# file (usually named Doxyfile):
# SAMRAI_DOXYGEN_LOCATION
# SAMRAI_DOXYGEN_TAGFILE
# SAMRAI_DOXYGEN_INPUT
# SAMRAI_DOXYGEN_FILE_PATTERNS
# SAMRAI_DOXYGEN_EXCLUDE_PATTERNS
# SAMRAI_DOXYGEN_RECURSIVE
# SAMRAI_DOXYGEN_PREDEFINED
AC_ARG_ENABLE(samrai-dox,
[  --enable-samrai-dox=LOCATION
			Include SAMRAI sources in doxygen documentation.
			LOCATION should be a full path or URL to where
			the SAMRAI doxygen documentation resides.
			If omitted, it points to the one in the
			docs/doxygen/html directory where SAMRAI is compiled.],,
enable_samrai_dox=no)

# Set the SAMRAI doxygen location to the default if necessary.
case "$enable_samrai_dox" in
  no) unset SAMRAI_DOXYGEN_LOCATION ;;
  yes)
    # Expect SAMRAI doxygen to be in the compile directory, under docs directory.
    # Error if SAMRAI_COMPILE is null.
    if test -z "${SAMRAI_COMPILE}"; then
      AC_MSG_ERROR(
[You must specify --with-samrai-compile=PATH if you do not specify
SAMRAI doxygen documentation location using --enable-samrai-dox=LOCATION.])
    fi
    SAMRAI_DOXYGEN_LOCATION="${SAMRAI_COMPILE}/docs/samrai-dox/html"
  ;;
  *) SAMRAI_DOXYGEN_LOCATION="$enable_samrai_dox"
esac

SAMRAI_DOXYGEN_INPUT=
SAMRAI_DOXYGEN_FILE_PATTERNS=
SAMRAI_DOXYGEN_EXCLUDE_PATTERNS=
if test "${SAMRAI_DOXYGEN_LOCATION+set}" = set ; then
  # Set up substitutions that can be used to set up a Doxyfile
  # to link to the SAMRAI documentation.
  # If the user specified development documentation, modify some variables
  #   to reflect that.
  SAMRAI_DOXYGEN_TAGFILE="${SAMRAI_DOXYGEN_LOCATION}/samrai.tag"
  SAMRAI_DOXYGEN_INPUT="${SAMRAI_SRC}/source/algorithm ${SAMRAI_SRC}/source/geometry ${SAMRAI_SRC}/source/hierarchy ${SAMRAI_SRC}/source/patchdata ${SAMRAI_SRC}/source/mesh ${SAMRAI_SRC}/source/solvers ${SAMRAI_SRC}/source/toolbox ${SAMRAI_SRC}/source/transfer ${SAMRAI_SRC}/source/mathops ${SAMRAI_SRC}/source/plotting"
  SAMRAI_DOXYGEN_FILE_PATTERNS=['*.[ChI] *.[ChI].sed']
  SAMRAI_DOXYGEN_EXCLUDE_PATTERNS='*/[[123]]d/*'
  SAMRAI_DOXYGEN_RECURSIVE=YES
  SAMRAI_DOXYGEN_PREDEFINED='NDIM=3 HAVE_KINSOL HAVE_HYPRE HAVE_PETSC'
  # Create the URL string.
  SAMRAI_DOXYGEN_URL="${SAMRAI_DOXYGEN_LOCATION}"
  case "${SAMRAI_DOXYGEN_URL}" in /*) SAMRAI_DOXYGEN_URL="file:${SAMRAI_DOXYGEN_URL}"; esac
  # Warn if SAMRAI doxygen not been generated.
  if test ! -r "${SAMRAI_DOXYGEN_TAGFILE}" ; then
    AC_MSG_WARN(
[File ${SAMRAI_DOXYGEN_TAGFILE} not readable!
Maybe (cd ${SAMRAI_COMPILE}/doc && make dox) required?])
  fi
fi

AC_CONFIG_COMMANDS([samrai_tag],[
if test -r "${SAMRAI_DOXYGEN_TAGFILE}"; then
  cp -f "${SAMRAI_DOXYGEN_TAGFILE}" .
  BTNG_AC_LOG(copied file ${SAMRAI_DOXYGEN_TAGFILE} to `pwd`)
fi
],[
# Set the location of SAMRAI doxygen documentation directory.
SAMRAI_DOXYGEN_LOCATION="${SAMRAI_DOXYGEN_LOCATION}"
])

AC_SUBST(SAMRAI_DOXYGEN_LOCATION)
AC_SUBST(SAMRAI_DOXYGEN_URL)
AC_SUBST(SAMRAI_DOXYGEN_TAGFILE)
AC_SUBST(SAMRAI_DOXYGEN_INPUT)
AC_SUBST(SAMRAI_DOXYGEN_FILE_PATTERNS)
AC_SUBST(SAMRAI_DOXYGEN_EXCLUDE_PATTERNS)
AC_SUBST(SAMRAI_DOXYGEN_RECURSIVE)
AC_SUBST(SAMRAI_DOXYGEN_PREDEFINED)
BTNG_AC_LOG_VAR(enable_samrai_dox
SAMRAI_DOXYGEN_LOCATION
SAMRAI_DOXYGEN_URL
SAMRAI_DOXYGEN_TAGFILE
SAMRAI_DOXYGEN_INPUT
SAMRAI_DOXYGEN_FILE_PATTERNS
SAMRAI_DOXYGEN_EXCLUDE_PATTERNS
SAMRAI_DOXYGEN_RECURSIVE
SAMRAI_DOXYGEN_PREDEFINED)

])


