/*
*********************************************************************************************************
*                                             uC/GUI V3.98
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              礐/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : GUIConf.h
Purpose     : Configures abilities, fonts etc.
----------------------------------------------------------------------
*/


#ifndef GUICONF_H 
#define GUICONF_H

#define GUI_OS                    (0)  /* Compile with multitasking support */
#define GUI_SUPPORT_TOUCH         (1)  /* 支持触摸屏(req. win-manager) */
#define GUI_SUPPORT_MOUSE         (1)  /* 支持鼠标*/
#define GUI_SUPPORT_UNICODE       (1)  /* Support mixed ASCII/UNICODE strings */

#define GUI_DEFAULT_FONT          &GUI_Font8x16
#define GUI_ALLOC_SIZE            (512)  /* Size of dynamic memory ... For WM and memory devices*/

#define GUI_MAXTASK                4
//#define GUI_DEFAULT_BKCOLOR        0x0000;
//#define GUI_DEFAULT_COLOR          0xFFFF;
/*********************************************************************
*
*         Configuration of available packages
*/

#define GUI_WINSUPPORT            1  /* Window manager package available */
#define GUI_SUPPORT_MEMDEV        1  /* Memory devices available */
#define GUI_SUPPORT_AA            1  /* Anti aliasing available */

#endif  /* Avoid multiple inclusion */


	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
 
