/*
 * Copyright (C) 2014 ISIR
 * Authors: Guillaume Hamon
*/
//http://www.gtkforums.com/viewtopic.php?t=3301#8764
//#include "iCub/skinManagerGui/guiCallback.h"
#include <string>
#include <sstream>
#include <gtk/gtk.h>
#include <stdlib.h>

#include <yarp/os/Network.h>
#include <yarp/os/RFModule.h>
#include <yarp/sig/Vector.h>
#include <yarp/os/all.h>

using namespace yarp::os;
using namespace std;

int intcurve, i;
float * curvetoplot;
float * curvetoplot2;
float * curvetoplot3;
cairo_t * cr;
cairo_t * cr2;
cairo_t * cr3;
yarp::os::BufferedPort<yarp::os::Bottle> jtsPort;
double ordonnee_RA[3];
GtkWidget* pWindow;
GtkWidget* darea;
GtkWidget* darea2;
GtkWidget* darea3;

static gboolean on_expose_event(gpointer data)
{


	Bottle* in_RA;
     	in_RA=jtsPort.read(true);


	cr = gdk_cairo_create(darea->window);
	cairo_set_source_rgb (cr, 1, 1, 1);
	cairo_paint (cr);
	cairo_set_source_rgb (cr, 1.0, 0.0, 0.0);
	cairo_set_line_width (cr, 5.0);

	cr2 = gdk_cairo_create(darea2->window);
	cairo_set_source_rgb (cr2, 1, 1, 1);
	cairo_paint (cr2);
	cairo_set_source_rgb (cr2, 1.0, 0.0, 0.0);
	cairo_set_line_width (cr2, 5.0);

	cr3 = gdk_cairo_create(darea3->window);
	cairo_set_source_rgb (cr3, 1, 1, 1);
	cairo_paint (cr3);
	cairo_set_source_rgb (cr3, 1.0, 0.0, 0.0);
	cairo_set_line_width (cr3, 5.0);

     	if (in_RA!=NULL) {
		for(int i=0; i<3;i++){
			ordonnee_RA[i]=((*in_RA).get(i).asDouble());
			ordonnee_RA[i]/=15.0;
			ordonnee_RA[i]*=150;
		}
       	}
	for(int i=100;i>0;i--){
		curvetoplot[i]=curvetoplot[i-1];
		curvetoplot2[i]=curvetoplot2[i-1];
		curvetoplot3[i]=curvetoplot3[i-1];
	}
	curvetoplot[0]=ordonnee_RA[0];
	curvetoplot2[0]=ordonnee_RA[1];
	curvetoplot3[0]=ordonnee_RA[2];

	for ( i=0 ; i < intcurve-1 ; i++ )
	{
    		cairo_move_to(cr, i*6, -curvetoplot[i]+170); // échelle: +15/-15 600x300
    		cairo_line_to(cr, (i+1)*6, -curvetoplot[i+1]+170);
		cairo_move_to(cr2, i*6, -curvetoplot2[i]+170); // échelle: +15/-15 600x300
    		cairo_line_to(cr2, (i+1)*6, -curvetoplot2[i+1]+170);
		cairo_move_to(cr3, i*6, -curvetoplot3[i]+170); // échelle: +15/-15 600x300
    		cairo_line_to(cr3, (i+1)*6, -curvetoplot3[i+1]+170);
 	}
	cairo_stroke(cr);
	cairo_stroke(cr2);
	cairo_stroke(cr3);
	cairo_destroy(cr);
	cairo_destroy(cr2);
	cairo_destroy(cr3);
	return true;
}



void OnDestroy(GtkWidget *pWidget, gpointer pData)
{
    /* Arret de la boucle evenementielle */
    gtk_main_quit();
}

int main (int argc, char *argv[])
{
 	Network yarp;
  	ResourceFinder rf;
	GtkWidget *pLabel, *pLabel2, *pLabel3;
	GtkWidget *pImage, *pImage2, *pImage3;
	GtkWidget *pHBox;
	GtkWidget *pHBox2;
	GtkWidget *pHBox3;
	GtkWidget *pVBox;

    /***********string guiName;***************/
	intcurve=100;
	curvetoplot=(float*)malloc(100*sizeof(float));
	curvetoplot2=(float*)malloc(100*sizeof(float));
	curvetoplot3=(float*)malloc(100*sizeof(float));
	for( i=0 ; i < intcurve ; i++ ){
		curvetoplot[i]=0.0;
		curvetoplot2[i]=0.0;
		curvetoplot3[i]=0.0;
	}

	if(!jtsPort.open("/jtsCalibrationGUI/jts:i")){
		cout << ": unable to open input port" << endl;
		return false; // unable to open; let RFModule know so that it won't run
     	}

	gtk_idle_add(on_expose_event, NULL);
	gtk_init (&argc, &argv);    // initialize gtk

//   	if(!initNetwork(yarp, rf, argc, argv, guiName, gXpos, gYpos))
//      return 0;

    	pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    	gtk_window_set_default_size(GTK_WINDOW(pWindow), 820, 900);
 // 	gtk_window_set_resizable(GTK_WINDOW(pWindow), true);
    	gtk_window_set_title(GTK_WINDOW(pWindow), "Joints Torque Sensors Calibration GUI");

	pVBox = gtk_vbox_new(TRUE, 0);
        gtk_container_add(GTK_CONTAINER(pWindow), pVBox);

	pHBox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(pVBox), pHBox, FALSE, TRUE, 0);

	pImage= gtk_image_new_from_file("/home/guillaume/icub-main/src/tools/jtsCalibrationGUI/src/echelle.png");
	pLabel = gtk_label_new("0");
	gtk_box_pack_start(GTK_BOX(pHBox), pImage, FALSE, TRUE, 0);
	darea = gtk_drawing_area_new();
	gtk_box_pack_start(GTK_BOX(pHBox), darea, TRUE, TRUE, 0);
	pLabel = gtk_label_new("Right Arm Joint 0 (N)");
	gtk_box_pack_start(GTK_BOX(pHBox), pLabel, FALSE, TRUE, 0);


	pHBox2 = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(pVBox), pHBox2, FALSE, TRUE, 0);

	pImage2= gtk_image_new_from_file("/home/guillaume/icub-main/src/tools/jtsCalibrationGUI/src/echelle.png");
	gtk_box_pack_start(GTK_BOX(pHBox2), pImage2, FALSE, TRUE, 0);
	darea2 = gtk_drawing_area_new();
	gtk_box_pack_start(GTK_BOX(pHBox2), darea2, TRUE, TRUE, 0);
	pLabel2 = gtk_label_new("Right Arm Joint 1 (N)");
	gtk_box_pack_start(GTK_BOX(pHBox2), pLabel2, FALSE, TRUE, 0);


	pHBox3 = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(pVBox), pHBox3, FALSE, TRUE, 0);

	pImage3= gtk_image_new_from_file("/home/guillaume/icub-main/src/tools/jtsCalibrationGUI/src/echelle.png");
	gtk_box_pack_start(GTK_BOX(pHBox3), pImage3, FALSE, TRUE, 0);
	darea3 = gtk_drawing_area_new();
	gtk_box_pack_start(GTK_BOX(pHBox3), darea3, TRUE, TRUE, 0);
	pLabel3 = gtk_label_new("Right Arm Joint 3 (N)");
	gtk_box_pack_start(GTK_BOX(pHBox3), pLabel3, FALSE, TRUE, 0);

  	gtk_widget_show_all(GTK_WIDGET(pWindow));
	std::cout<< "gtk_main"<<std::endl;


 	gtk_main ();


//  	gdk_threads_leave();


    	return 0;
}
