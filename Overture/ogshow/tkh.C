#include <stdio.h>
#include <iostream.h>
#include <stdlib.h>
extern "C"
{
#include <tcl.h>
#include <tk.h>
}

int Tcl_AppInit(Tcl_Interp *interp)
{
  if( Tcl_Init(interp) == TCL_ERROR)
  {
    return TCL_ERROR;
  }
  return TCL_OK;
}

//int __main()
//{
//}


int main(int argc, char *argv[])
{
	Tk_Window mainWindow;
	Tcl_Interp *interp;
	static char *display = NULL;

	interp = Tcl_CreateInterp();

	mainWindow = Tk_CreateMainWindow(interp, display, argv[0], "Tk");
	if (mainWindow == NULL) {
		fprintf(stderr, "%s\n", interp->result);
		exit(1);
	}

	if (Tcl_Init(interp) == TCL_ERROR) {
		fprintf(stderr, "Tcl_Init failed: %s\n", interp->result);
	}
	if (Tk_Init(interp) == TCL_ERROR) {
		fprintf(stderr, "Tk_Init failed: %s\n", interp->result);
	}

	Tcl_Eval(interp, "button .b -text \"Hello World\" -command exit");
	Tcl_Eval(interp, "pack .b");
	Tk_MainLoop();
	exit(1);
}
