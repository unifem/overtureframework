#ifndef _GraphicsInterfaceF
#define _GraphicsInterfaceF

#ifdef NO_UNDER
//
// Change the Fortran subroutine and function names to accomodate
// Fortran compilers that do not append an underscore to the names.
//
#define giopph_ giopph
#define giclph_ giclph
#define giopws_ giopws
#define giscws_ giscws
#define giqwsa_ giqwsa
#define giupws_ giupws
#define giupw_  giupw
#define giclws_ giclws
#define giivf_  giivf
#define giview_ giview
#define givw_   givw
#define giarv_  giarv
#define giarvw_ giarvw
#define gidrv_  gidrv
#define gidrvw_ gidrvw
#define girast_ girast
#define girasw_ girasw
#define gialst_ gialst
#define gicci_  gicci
#define gishad_ gishad
#define gidlst_ gidlst
// #define gimenu_ gimenu
#define giin_   giin
#define giind_  giind
#define gicrsr_ gicrsr
#define giout_  giout
#define gienv_  gienv
#define gidbug_ gidbug
#define girdcf_ girdcf
#define gisvcf_ gisvcf
#define gigtin_ gigtin
#define gigtre_ gigtre
#define gidcpy_ gidcpy
#define giddel_ giddel
#define gidfnd_ gidfnd
#define gidlnk_ gidlnk
#define gidout_ gidout
#define gidrel_ gidrel
#define gidump_ gidump
#define gispvw_ gispvw
#endif // NO_UNDER

#ifdef CRAY
//
// Capitalize the Fortran subroutine and function names to accomodate
// the Fortran compiler, which always converts the names to upper case.
//
#define giopph_ GIOPPH
#define giclph_ GICLPH
#define giopws_ GIOPWS
#define giscws_ GISCWS
#define giqwsa_ GIQWSA
#define giupws_ GIUPWS
#define giupw_  GIUPW
#define giclws_ GICLWS
#define giivf_  GIIVF
#define giview_ GIVIEW
#define givw_   GIVW
#define giarv_  GIARV
#define giarvw_ GIARVW
#define gidrv_  GIDRV
#define gidrvw_ GIDRVW
#define girast_ GIRASW
#define girasw_ GIRASW
#define gialst_ GIALST
#define gicci_  GICCI
#define gishad_ GISHAD
#define gidlst_ GIDLST
// #define gimenu_ GIMENU
#define giin_   GIIN
#define giind_  GIIND
#define gicrsr_ GICRSR
#define giout_  GIOUT
#define gienv_  GIENV
#define gidbug_ GIDBUG
#define girdcf_ GIRDCF
#define gisvcf_ GISVCF
#define gigtin_ GIGTIN
#define gigtre_ GIGTRE
#define gidcpy_ GIDCPY
#define giddel_ GIDDEL
#define gidfnd_ GIDFND
#define gidlnk_ GIDLNK
#define gidout_ GIDOUT
#define gidrel_ GIDREL
#define gidump_ GIDUMP
#define gispvw_ GISPVW

extern "C" {
    void giopph_(Int* id_, Float* rd_, Int& q0_, const Int& dir,
      const _fcd flags, const Int& ip, const _fcd com, const Int& ncom);
    void giclph_(Int* id_, const Int& q0_, const _fcd message);
    void giopws_(Int* id_, const Int& q0_, const Int& wsid);
    void giscws_(Int* id_, const Int& q0_, const Int& iogr, const Int& wsid);
    void giqwsa_(Int* id_, Float* rd_, const Int& q0_, Float area1[2][2],
      Float area2[2][2]);
    void giupws_(Int* id_, Float* rd_, const Int& q0_, const Int& regen);
    void giupw_(Int* id_, Float* rd_, const Int& q0_, const Int& wsid,
      const Int& regen);
    void giclws_(Int* id_, const Int& q0_, const Int& wsid);
    void giivf_(Int* id_, const Int& q0_, const _fcd flags, const Int& ip);
    void giview_(Int* id_, Float* rd_, const Int& q0_, const _fcd flags,
      const Int& ip, const Float& rp);
    void givw_(Int* id_, Float* rd_, const Int& q0_, const Int& wsid,
      const _fcd flags, const Int& ip, const Float& rp);
    void giarv_(Int* id_, const Int& q0_, const Int& view, const Int& strid);
    void giarvw_(Int* id_, const Int& q0_, const Int& wsid, const Int& view,
      const Int& strid);
    void gidrv_(Int* id_, const Int& q0_, const Int& view, const Int& strid);
    void gidrvw_(Int* id_, const Int& q0_, const Int& wsid, const Int& view,
      const Int& strid);
    void girast_(Int* id_, const Int& q0_, const Int& when);
    void girasw_(Int* id_, const Int& q0_, const Int& wsid, const Int& when);
    void gialst_(Int* id_, const Int& q0_, Int& strid);
    void gicci_(Int* id_, const Int& q0_, const _fcd colour, Int& index,
      Int& ierr);
    void gishad_(Int* id_, Float* rd_, const Int& q0_, const Int& strid,
     const Float& shade);
    void gidlst_(Int* id_, const Int& q0_, const Int& strid);
//  void gimenu_(Int* id_, const Int& q0_, const _fcd menu, const Int& nmenu,
//   _fcd answer, Int& ierr);
    void giin_(Int* id_, const Int& q0_, const _fcd menu, const Int& nmenu,
      const _fcd prompt, _fcd answer);
    void giind_(Int* id_, const Int& q0_, const Int& dir, const _fcd menu,
      const Int& nmenu, const _fcd prompt, _fcd answer);
    void gicrsr_(Int* id_, const Int& q0_, const Int& view, const _fcd menu,
      const Int& nmenu, const _fcd prompt, _fcd answer, Float x[3]);
    void giout_(Int* id_, const Int& q0_, const Int& severity,
      const _fcd message);
    void gienv_(Int* id_, const Int& q0_, const _fcd flags, const Int& ip);
    void gidbug_(Int* id_, const Int& q0_,
      const _fcd routine, const _fcd arg1, const _fcd arg2);
    void girdcf_(Int* id_, const Int& q0_);
    void gisvcf_(Int* id_, const Int& q0_);
    void gigtin_(Int* id_, const Int& q0_, _fcd line, const Int in[],
      const Int& n, Int& ierr);
    void gigtre_(Int* id_, const Int& q0_, const _fcd line, const Float x[],
      const Int& n, Int& ierr);
    void gidcpy_(Int* id_, const Int& q0_, const Int& dir1, const _fcd name1,
     const Int& dir2, const _fcd name2, const _fcd flags);
    void giddel_(Int* id_, const Int& q0_, const Int& dir, const _fcd name,
      const _fcd flags);
    Int  gidfnd_(Int* id_, const Int& q0_, const Int& dir, const _fcd name);
    void gidlnk_(Int* id_, const Int& q0_, const Int& dir, const _fcd name,
      const _fcd type, const Int& dims, const Int& loc);
    void gidout_(Int* id_, const Int& q0_, const Int& dir, const _fcd name,
      const _fcd flags, const Int& unit);
    void gidrel_(Int* id_, const Int& q0_, const Int& dir, const _fcd name,
      const _fcd flags);
    void gidump_(Int* id_, const Int& q0_);
    void gispvw_(Int* id_, const Int& q0_, const Int& view0);
}

//
// Convert calls that pass character strings and their lengths to instead
// pass CRAY Fortran character descriptors.
//
extern "C" {
inline void giopph_(Int* id_, Float* rd_, Int& q0_, const Int& dir,
  const char* flags, const Int& ip, const char* com, const Int& ncom,
  const Int len_flags, const Int len_com) {
    giopph_(id_, rd_, q0_, dir, _cptofcd(flags,len_flags), ip,
      _cptofcd(com,len_com), ncom);
}
inline void giclph_(Int* id_, const Int& q0_, const char* message,
  const Int len_message) {
    giclph_(id_, q0_, _cptofcd(message,len_message));
}
inline void giivf_(Int* id_, const Int& q0_, const char* flags, const Int& ip,
  const Int len_flags) {
    giivf_(id_, q0_, _cptofcd(flags,len_flags), ip);
}
inline void giview_(Int* id_, Float* rd_, const Int& q0_, const char* flags,
  const Int& ip, const Float& rp, const Int len_flags) {
    giview_(id_, rd_, q0_, _cptofcd(flags,len_flags), ip, rp);
}
inline void givw_(Int* id_, Float* rd_, const Int& q0_, const Int& wsid,
  const char* flags, const Int& ip, const Float& rp, const Int len_flags) {
    givw_(id_, rd_, q0_, wsid, _cptofcd(flags,len_flags), ip, rp);
}
inline void gicci_(Int* id_, const Int& q0_, const char* colour, Int& index,
  Int& ierr, const Int len_colour) {
    gicci_(id_, q0_, _cptofcd(colour,len_colour), index, ierr);
}
// inline void gimenu_(Int* id_, const Int& q0_, const char* menu,
//   const Int& nmenu, char* answer, Int& ierr, const Int len_menu,
//   const Int len_answer);
//     gimenu_(id_, q0_, _cptofcd(menu,len_menu), nmenu,
//     _cptofcd(answer,len_answer), ierr);
inline void giin_(Int* id_, const Int& q0_, const char* menu, const Int& nmenu,
  const char* prompt, char* answer,
  const Int len_menu, const Int len_prompt, const Int len_answer) {
    giin_(id_, q0_, _cptofcd(menu,len_menu), nmenu, _cptofcd(prompt,len_prompt),
      _cptofcd(answer,len_answer));
}
inline void giind_(Int* id_, const Int& q0_, const Int& dir, const char* menu,
  const Int& nmenu, const char* prompt, char* answer,
  const Int len_menu, const Int len_prompt, const Int len_answer) {
    giind_(id_, q0_, dir, _cptofcd(menu,len_menu), nmenu,
      _cptofcd(prompt,len_prompt), _cptofcd(answer,len_answer));
}
inline void gicrsr_(Int* id_, const Int& q0_, const Int& view, const char* menu,
  const Int& nmenu, const char* prompt, char* answer, Float x[3],
  const Int len_menu, const Int len_prompt, const Int len_answer);
    gicrsr_(id_, q0_, view, _cptofcd(menu,len_menu), nmenu,
      _cptofcd(prompt,len_prompt), _cptofcd(answer,len_answer), x);
inline void giout_(Int* id_, const Int& q0_, const Int& severity,
  const char* message, const Int len_message) {
    giout_(id_, q0_, severity, _cptofcd(message,len_message));
}
inline void gienv_(Int* id_, const Int& q0_, const char* flags, const Int& ip,
  const Int len_flags) {
    gienv_(id_, q0_, _cptofcd(flags,len_flags), ip);
}
inline void gidbug_(Int* id_, const Int& q0_,
  const char* routine, const char* arg1, const char* arg2,
  const Int len_routine, const Int len_arg1, const Int len_arg2) {
    gidbug_(id_, q0_, _cptofcd(routine,len_routine), _cptofcd(arg1,len_arg1),
      _cptofcd(arg2,len_arg2));
}
inline void gigtin_(Int* id_, const Int& q0_, const char* line, const Int in[],
  const Int& n, Int& ierr, const Int len_line) {
    gigtin_(id_, q0_, _cptofcd(line,len_line), in, n, ierr);
}
inline void gigtre_(Int* id_, const Int& q0_, const char* line, const Float x[],
  const Int& n, Int& ierr, const Int len_line) {
    gigtre_(id_, q0_, _cptofcd(line,len_line), x, n, ierr);
}
inline void gidcpy_(Int* id_, const Int& q0_, const Int& dir1,
  const char* name1, const Int& dir2, const char* name2, const char* flags,
  const Int len_name1, const Int len_name2, const Int len_flags) {
    gidcpy_(id_, q0_, dir1, _cptofcd(name1,len_name1), dir2,
      _cptofcd(name2,len_name2), _cptofcd(flags,len_flags));
}
inline void giddel_(Int* id_, const Int& q0_, const Int& dir, const char* name,
  const char* flags, const Int len_name, const Int len_flags) {
    giddel_(id_, q0_, dir, _cptofcd(name,len_name), _cptofcd(flags,len_flags));
}
inline Int  gidfnd_(Int* id_, const Int& q0_, const Int& dir, const char* name,
  const Int len_name) {
    return gidfnd_(id_, q0_, dir, _cptofcd(name,len_name));
}
inline void gidlnk_(Int* id_, const Int& q0_, const Int& dir, const char* name,
  const char* type, const Int& dims, const Int& loc,
  const Int len_name, const Int len_type) {
    gidlnk_(id_, q0_, dir, _cptofcd(name,len_name), _cptofcd(type,len_type),
      dims, loc);
}
inline void gidout_(Int* id_, const Int& q0_, const Int& dir, const char* name,
  const char* flags, const Int& unit, const Int len_name,
  const Int len_flags) {
    gidout_(id_, q0_, dir, _cptofcd(name,len_name), _cptofcd(flags,len_flags),
      unit);
}
inline void gidrel_(Int* id_, const Int& q0_, const Int& dir, const char* name,
  const char* flags, const Int len_name, const Int len_flags) {
    gidrel_(id_, q0_, dir, _cptofcd(name,len_name), _cptofcd(flags,len_flags));
}

#else
extern "C" {
    void giopph_(Int* id_, Float* rd_, Int& q0_, const Int& dir,
      const char* flags, const Int& ip, const char* com, const Int& ncom,
      const Int len_flags, const Int len_com);
    void giclph_(Int* id_, const Int& q0_, const char* message,
      const Int len_message);
    void giopws_(Int* id_, const Int& q0_, const Int& wsid);
    void giscws_(Int* id_, const Int& q0_, const Int& iogr, const Int& wsid);
    void giqwsa_(Int* id_, Float* rd_, const Int& q0_, Float area1[2][2],
      Float area2[2][2]);
    void giupws_(Int* id_, Float* rd_, const Int& q0_, const Int& regen);
    void giupw_(Int* id_, Float* rd_, const Int& q0_, const Int& wsid,
      const Int& regen);
    void giclws_(Int* id_, const Int& q0_, const Int& wsid);
    void giivf_(Int* id_, const Int& q0_, const char* flags, const Int& ip,
      const Int len_flags);
    void giview_(Int* id_, Float* rd_, const Int& q0_, const char* flags,
      const Int& ip, const Float& rp, const Int len_flags);
    void givw_(Int* id_, Float* rd_, const Int& q0_, const Int& wsid,
      const char* flags, const Int& ip, const Float& rp, const Int len_flags);
    void giarv_(Int* id_, const Int& q0_, const Int& view, const Int& strid);
    void giarvw_(Int* id_, const Int& q0_, const Int& wsid, const Int& view,
      const Int& strid);
    void gidrv_(Int* id_, const Int& q0_, const Int& view, const Int& strid);
    void gidrvw_(Int* id_, const Int& q0_, const Int& wsid, const Int& view,
      const Int& strid);
    void girast_(Int* id_, const Int& q0_, const Int& when);
    void girasw_(Int* id_, const Int& q0_, const Int& wsid, const Int& when);
    void gialst_(Int* id_, const Int& q0_, Int& strid);
    void gicci_(Int* id_, const Int& q0_, const char* colour, Int& index,
      Int& ierr, const Int len_colour);
    void gishad_(Int* id_, Float* rd_, const Int& q0_, const Int& strid,
     const Float& shade);
    void gidlst_(Int* id_, const Int& q0_, const Int& strid);
//  void gimenu_(Int* id_, const Int& q0_, const char* menu, const Int& nmenu,
//   char* answer, Int& ierr, const Int len_menu, const Int len_answer);
    void giin_(Int* id_, const Int& q0_, const char* menu, const Int& nmenu,
      const char* prompt, char* answer,
      const Int len_menu, const Int len_prompt, const Int len_answer);
    void giind_(Int* id_, const Int& q0_, const Int& dir, const char* menu,
      const Int& nmenu, const char* prompt, char* answer,
      const Int len_menu, const Int len_prompt, const Int len_answer);
    void gicrsr_(Int* id_, const Int& q0_, const Int& view, const char* menu,
      const Int& nmenu, const char* prompt, char* answer, Float x[3],
      const Int len_menu, const Int len_prompt, const Int len_answer);
    void giout_(Int* id_, const Int& q0_, const Int& severity,
      const char* message, const Int len_message);
    void gienv_(Int* id_, const Int& q0_, const char* flags, const Int& ip,
      const Int len_flags);
    void gidbug_(Int* id_, const Int& q0_,
      const char* routine, const char* arg1, const char* arg2,
      const Int len_routine, const Int len_arg1, const Int len_arg2);
    void girdcf_(Int* id_, const Int& q0_);
    void gisvcf_(Int* id_, const Int& q0_);
    void gigtin_(Int* id_, const Int& q0_, const char* line, const Int in[],
      const Int& n, Int& ierr, const Int len_line);
    void gigtre_(Int* id_, const Int& q0_, const char* line, const Float x[],
      const Int& n, Int& ierr, const Int len_line);
    void gidcpy_(Int* id_, const Int& q0_, const Int& dir1, const char* name1,
     const Int& dir2, const char* name2, const char* flags,
     const Int len_name1, const Int len_name2, const Int len_flags);
    void giddel_(Int* id_, const Int& q0_, const Int& dir, const char* name,
      const char* flags, const Int len_name, const Int len_flags);
    Int  gidfnd_(Int* id_, const Int& q0_, const Int& dir, const char* name,
      const Int len_name);
    void gidlnk_(Int* id_, const Int& q0_, const Int& dir, const char* name,
      const char* type, const Int& dims, const Int& loc,
      const Int len_name, const Int len_type);
    void gidout_(Int* id_, const Int& q0_, const Int& dir, const char* name,
      const char* flags, const Int& unit, const Int len_name,
      const Int len_flags);
    void gidrel_(Int* id_, const Int& q0_, const Int& dir, const char* name,
      const char* flags, const Int len_name, const Int len_flags);
    void gidump_(Int* id_, const Int& q0_);
    void gispvw_(Int* id_, const Int& q0_, const Int& view0);
}
#endif // CRAY

#endif // _GraphicsInterfaceF
