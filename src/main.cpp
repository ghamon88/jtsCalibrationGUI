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
yarp::os::BufferedPort<yarp::os::Bottle> jtsPort_RA;
double ordonnee_RA[3];
GtkWidget* pWindow;
GtkWidget* darea; 
GtkWidget* darea2; 
GtkWidget* darea3; 

static gboolean on_expose_event(gpointer data)
{
	

	Bottle* in_RA;
     	in_RA=jtsPort_RA.read(true);
        
	
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

	if(!jtsPort_RA.open("/jtsCalibrationGUI/ra_jts:i")){
		cout << ": unable to open input port RA" << endl;
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


/*bool initGuiStatus(){
    Bottle reply = sendRpcCommand(true, get_binarization);
    if(string(reply.toString().c_str()).compare("on") == 0){
        gtk_toggle_button_set_active(btnBinarization, true);
        gtk_button_set_label(GTK_BUTTON(btnBinarization), "ON");
    }

    reply = sendRpcCommand(true, get_smooth_filter);
    if(string(reply.toString().c_str()).compare("on") == 0){
        gtk_toggle_button_set_active(btnSmooth, true);
        gtk_button_set_label(GTK_BUTTON(btnSmooth), "ON");
        gtk_widget_set_sensitive(GTK_WIDGET(scaleSmooth), true);
    }else{
        gtk_widget_set_sensitive(GTK_WIDGET(scaleSmooth), false);
    }

    reply = sendRpcCommand(true, get_smooth_factor);
    currentSmoothFactor = reply.get(0).asDouble();
    gtk_adjustment_set_value(scaleSmooth->range.adjustment, currentSmoothFactor);   

    reply = sendRpcCommand(true, get_threshold);
    if(reply.isNull() || reply.size()==0 || !reply.get(0).isInt()){
        printLog("Error while getting the safety threshold");
        return false;
    }else{
        currentThreshold = reply.get(0).asInt();
        gtk_adjustment_set_value(spinThreshold->adjustment, currentThreshold);
    }

    reply = sendRpcCommand(true, get_gain);
    if(reply.isNull() || reply.size()==0 || (!reply.get(0).isDouble() && !reply.get(0).isInt())){
        printLog("Error while getting the compensation gain");
        return false;
    }else{
        currentCompGain = reply.get(0).asDouble();
        gtk_adjustment_set_value(spinGain->adjustment, currentCompGain);
    }

    reply = sendRpcCommand(true,get_cont_gain);
    if(reply.isNull() || reply.size()==0 || (!reply.get(0).isDouble() && !reply.get(0).isInt())){
        printLog("Error while getting the contact compensation gain");
        return false;
    }else{
        currentContCompGain = reply.get(0).asDouble();
        gtk_adjustment_set_value(spinContGain->adjustment, currentContCompGain);
    } 

    reply = sendRpcCommand(true,get_max_neigh_dist);
    if(reply.isNull() || reply.size()==0 || (!reply.get(0).isDouble() && !reply.get(0).isInt())){
        printLog("Error while getting the max neighbor distance");
        return false;
    }else{
        currentMaxNeighDist = reply.get(0).asDouble();
        gtk_adjustment_set_value(spinMaxNeighDist->adjustment, 1e2*currentMaxNeighDist);
    }

    // get module information
    reply = sendRpcCommand(true, get_info);
    if(reply.isNull() || reply.size()!=3){
        printLog("Error while reading the module information");
        gtk_label_set_text(lblInfo, reply.toString().c_str());
        return false;
    }
    stringstream ss;
    ss<< reply.get(0).toString().c_str()<< endl;
    ss<< reply.get(1).toString().c_str()<< "\nInput ports:";
    Bottle* portList = reply.get(2).asList();
    portNames.resize(portList->size()/2);
    portDim.resize(portList->size()/2);
    //int numTaxels = 0;
    for(unsigned int i=0;i<portDim.size();i++){
        portNames[i] = portList->get(i*2).toString().c_str();
        portDim[i] = portList->get(i*2+1).asInt();
        //numTaxels += portDim[i];
        ss<< "\n - "<< portNames[i]<< " ("<< portDim[i]<< " taxels)";
    }
    gtk_label_set_text(lblInfo, ss.str().c_str());

    // plot        
    unsigned int numTriangles = portDim[0]/12;    
    GtkTreeIter iter;    
    //gtk_label_set_text(lblMaxX, ss.str().c_str());
    for(unsigned int i=0;i<portDim.size();i++){
        gtk_list_store_append (listPort, &iter);
        gtk_list_store_set (listPort, &iter, 0, i, 1, portNames[i].c_str(), -1);
    }    
    for(unsigned int i=0;i<numTriangles;i++){
        gtk_list_store_append (listTriangle, &iter);
        gtk_list_store_set (listTriangle, &iter, 0, i, -1);
    }
    for(unsigned int i=0;i<12;i++){
        gtk_list_store_append (listTaxel, &iter);
        gtk_list_store_set (listTaxel, &iter, 0, i, -1);
    }
    gtk_combo_box_set_active(comboPort, 0);
    gtk_combo_box_set_active(comboTriangle, 0);
    gtk_combo_box_set_active(comboTaxel, 0);
    stringstream maxXS; maxXS<< currentSampleNum/currentSampleFreq;
    gtk_label_set_text(lblMaxX, maxXS.str().c_str());
    gtk_curve_set_range(curveComp, 0, (gfloat)currentSampleNum, 0, 255);

    // check whether the skin calibration is in process
    reply = sendRpcCommand(true, is_calibrating);
    if(string(reply.toString().c_str()).compare("yes")==0){
        gtk_widget_show(GTK_WIDGET(progBarCalib));
        g_timeout_add(100, progressbar_calibration, NULL);
        gtk_widget_set_sensitive(GTK_WIDGET(btnCalibration), false);
    }

    return true;
}*/

/*bool initNetwork(Network& yarp, ResourceFinder &rf, int argc, char *argv[], string &guiName, unsigned int& gXpos, unsigned int& gYpos){    
    rf.setVerbose(true);
    rf.setDefaultConfigFile("skinManGui.ini");      //overridden by --from parameter
    rf.setDefaultContext("skinGui");                    //overridden by --context parameter
    rf.configure(argc, argv);

    gXpos=10; 
    gYpos=10;
    if (rf.check("xpos")) gXpos=rf.find("xpos").asInt();
    if (rf.check("ypos")) gYpos=rf.find("ypos").asInt();

    string driftCompRpcPortName     = rf.check("skinManRpcPort", Value("/skinManager/rpc")).asString().c_str();
    string driftCompMonitorPortName = rf.check("skinManMonitorPort", Value("/skinManager/monitor:o")).asString().c_str();
    string driftCompInfoPortName    = rf.check("skinManInfoPort", Value("/skinManager/info:o")).asString().c_str();    
    guiName                         = rf.check("name", Value("skinManGui")).asString().c_str();
    string guiRpcPortName           = "/" + guiName + "/rpc:o";
    string guiMonitorPortName       = "/" + guiName + "/monitor:i";
    string guiInfoPortName          = "/" + guiName + "/info:i";
    //string wholeBodyRpcPortName       = "/" + guiName + "/wholeBody/rpc";
    if (!guiRpcPort.open(guiRpcPortName.c_str())) {
        string msg = string("Unable to open port ") + guiRpcPortName.c_str();
        openDialog(msg.c_str(), GTK_MESSAGE_ERROR);
        return false;
    }
    if (!driftCompMonitorPort.open(guiMonitorPortName.c_str())){
        string msg = string("Unable to open port ") + guiMonitorPortName.c_str();
        openDialog(msg.c_str(), GTK_MESSAGE_ERROR);
        return false;
    }
    if (!driftCompInfoPort.open(guiInfoPortName.c_str())){
        string msg = string("Unable to open port ") + guiInfoPortName.c_str();
        openDialog(msg.c_str(), GTK_MESSAGE_ERROR);
        return false;
    }
 //   if (!wholeBodyRpcPort.open(wholeBodyRpcPortName.c_str())){
    //  string msg = string("Unable to open port ") + wholeBodyRpcPortName.c_str();
    //  openDialog(msg.c_str(), GTK_MESSAGE_ERROR);
    //  return false;
    //}

    driftCompInfoPort.setStrict();
    
    // automatic connections removed because they gave problems when running the gui 
    // just after the skinDriftCompensation module (using manager.py)
    /*if(!yarp.connect(guiRpcPortName.c_str(), driftCompRpcPortName.c_str())){
        string msg = string("Unable to connect to skinDriftCompensation rpc port: ") 
            + driftCompRpcPortName.c_str() + ". Connect later.";
        openDialog(msg.c_str(), GTK_MESSAGE_WARNING);
    }
    if(!yarp.connect(driftCompMonitorPortName.c_str(), guiMonitorPortName.c_str())){
        string msg = string("Unable to connect to skinDriftCompensation monitor port: ") 
            + driftCompMonitorPortName.c_str() + ". Connect later.";
        openDialog(msg.c_str(), GTK_MESSAGE_WARNING);
    }
    if(!yarp.connect(driftCompInfoPortName.c_str(), guiInfoPortName.c_str())){
        string msg = string("Unable to connect to skinDriftCompensation info port: ") 
            + driftCompInfoPortName.c_str() + ". Connect later.";
        openDialog(msg.c_str(), GTK_MESSAGE_WARNING);
    }*/
    /*if(!yarp.connect(wholeBodyRpcPort.getName().c_str(), "/wholeBodyDynamics/rpc:i"))
        openDialog("Unable to connect to wholeBodyDynamics rpc port. Connect later.", GTK_MESSAGE_WARNING);*/
/*    return true;
}*/

