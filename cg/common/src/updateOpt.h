#ifndef UPDATE_OPT_H
#define UPDATE_OPT_H

#define updateOpt EXTERN_C_NAME(updateopt)
extern "C"
{
   void updateOpt(const int &nd1a,const int &nd1b,const int &nd2a,const int &nd2b,
                  const int &nd3a,const int &nd3b,const int &nd4a,const int &nd4b, \
		  const int &mask,real &u1, const real&u2,  
                  const real&ut1, const real&ut2, const real&ut3, const real&ut4, 
                  const int &ipar, const real& rpar, int & ierr );
}


#endif
