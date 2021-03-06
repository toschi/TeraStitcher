//------------------------------------------------------------------------------------------------
// Copyright (c) 2012  Alessandro Bria and Giulio Iannello (University Campus Bio-Medico of Rome).  
// All rights reserved.
//------------------------------------------------------------------------------------------------

/*******************************************************************************************************************************************************************************************
*    LICENSE NOTICE
********************************************************************************************************************************************************************************************
*    By downloading/using/running/editing/changing any portion of codes in this package you agree to this license. If you do not agree to this license, do not download/use/run/edit/change
*    this code.
********************************************************************************************************************************************************************************************
*    1. This material is free for non-profit research, but needs a special license for any commercial purpose. Please contact Alessandro Bria at a.bria@unicas.it or Giulio Iannello at 
*       g.iannello@unicampus.it for further details.
*    2. You agree to appropriately cite this work in your related studies and publications.
*    3. This material is provided by  the copyright holders (Alessandro Bria  and  Giulio Iannello),  University Campus Bio-Medico and contributors "as is" and any express or implied war-
*       ranties, including, but  not limited to,  any implied warranties  of merchantability,  non-infringement, or fitness for a particular purpose are  disclaimed. In no event shall the
*       copyright owners, University Campus Bio-Medico, or contributors be liable for any direct, indirect, incidental, special, exemplary, or  consequential  damages  (including, but not 
*       limited to, procurement of substitute goods or services; loss of use, data, or profits;reasonable royalties; or business interruption) however caused  and on any theory of liabil-
*       ity, whether in contract, strict liability, or tort  (including negligence or otherwise) arising in any way out of the use of this software,  even if advised of the possibility of
*       such damage.
*    4. Neither the name of University  Campus Bio-Medico of Rome, nor Alessandro Bria and Giulio Iannello, may be used to endorse or  promote products  derived from this software without
*       specific prior written permission.
********************************************************************************************************************************************************************************************/

#include "TemplateCLI.h"
#include "IM_config.h"
#include "StackedVolume.h"

#include <CmdLine.h>
#include <sstream>

TemplateCLI::TemplateCLI(void)
{
	// we use TCLAP default values to initialize most parameters (see readParams)

	// initialize other parameters that are not provided in the command line
	;
}

//reads options and parameters from command line
void TemplateCLI::readParams(int argc, char** argv) throw (iom::exception)
{
	//command line object definition
	TCLAP::CmdLine cmd(getHelpText(), '=', "2.2.0");
		/**
		 * Command line constructor. Defines how the arguments will be
		 * parsed.
		 * \param message - The message to be used in the usage
		 * output.
		 * \param delimiter - The character that is used to separate
		 * the argument flag/name from the value.  Defaults to ' ' (space).
		 * \param version - The version number to be used in the
		 * --version switch.
		 * \param helpAndVersion - Whether or not to create the Help and
		 * Version switches. Defaults to true.
		 */


	//argument objects definitions
	TCLAP::SwitchArg p_highest_resolution("a","all","Generate also the highest resolution.",false); 
        /**
		 * SwitchArg constructor.
		 * \param flag - The one character flag that identifies this
		 * argument on the command line.
		 * \param name - A one word name for the argument.  Can be
		 * used as a long flag on the command line.
		 * \param desc - A description of what the argument is for or
		 * does.
		 * \param def - The default value for this Switch. 
		 * \param v - An optional visitor.  You probably should not
		 * use this unless you have a very good reason.
		 */

	TCLAP::ValueArg<std::string> p_src_root_dir("s","src","Source file / root directory path.",true,"","string");
	TCLAP::ValueArg<std::string> p_dst_root_dir("d","dst","Destination root directory path.",true,"","string");
	TCLAP::ValueArg<int> p_slice_depth("","depth","Slice depth.",false,512,"unsigned");
	TCLAP::ValueArg<int> p_slice_height("","height","Slice height.",false,512,"unsigned");
	TCLAP::ValueArg<int> p_slice_width("","width","Slice width.",false,512,"unsigned");
	//TCLAP::ValueArg<string> p_src_format("","sfmt","Source format (Stacked/Simple/SimpleRaw/Raw/Tiled/TiledMC).",true,"","string");
	string temp = "Source format (\"" + 
		iim::TILED_MC_FORMAT + "\"/\"" + 
		iim::TILED_FORMAT + "\"/\"" + 
		iim::STACKED_FORMAT + "\"/\"" + 
		iim::SIMPLE_FORMAT + "\"/\"" + 
		iim::SIMPLE_RAW_FORMAT + "\"/\"" + 
		iim::RAW_FORMAT + "\"/\"" + 
		iim::TIF3D_FORMAT + "\"/\"" + 
		iim::TILED_TIF3D_FORMAT  + "\")";
	TCLAP::ValueArg<string> p_src_format("","sfmt",temp.c_str(),true,"","string");
	TCLAP::ValueArg<string> p_dst_format("","dfmt","Destination format (intensity/graylevel/RGB).",true,"","string");
 	TCLAP::ValueArg<int> p_n_resolutions("","res","Number of resolutions.",true,2,"unsigned");
	TCLAP::ValueArg<string> p_halving_method("","halve","Halving method (mean/max, default: mean).",false,"mean","unsigned");
	TCLAP::ValueArg<std::string> p_outFmt("f","outFmt","Output format (Tiff2DStck/Vaa3DRaw/Tiff3D/Vaa3DRawMC/Tiff3DMC, default: Tiff2DStck).",false,"Tiff2DStck","string");
       /**
         * Labeled ValueArg constructor.
         * You could conceivably call this constructor with a blank flag, 
         * but that would make you a bad person.  It would also cause
         * an exception to be thrown.   If you want an unlabeled argument, 
         * use the other constructor.
         * \param flag - The one character flag that identifies this
         * argument on the command line.
         * \param name - A one word name for the argument.  Can be
         * used as a long flag on the command line.
         * \param desc - A description of what the argument is for or
         * does.
         * \param req - Whether the argument is required on the command
         * line.
         * \param value - The default value assigned to this argument if it
         * is not present on the command line.
         * \param typeDesc - A short, human readable description of the
         * type that this object expects.  This is used in the generation
         * of the USAGE statement.  The goal is to be helpful to the end user
         * of the program.
         * \param v - An optional visitor.  You probably should not
         * use this unless you have a very good reason.
         */


	//argument objects must be inserted using LIFO policy (last inserted, first shown)
	cmd.add(p_outFmt);
	cmd.add(p_highest_resolution);
	cmd.add(p_n_resolutions);
	cmd.add(p_dst_format);
	cmd.add(p_src_format);
	cmd.add(p_slice_width);
	cmd.add(p_slice_height);
	cmd.add(p_slice_depth);
	cmd.add(p_dst_root_dir);
	cmd.add(p_src_root_dir);

	// Parse the argv array and catch <TCLAP> exceptions, which are translated into <MyException> exceptions
	char errMsg[S_STATIC_STRINGS_SIZE];
	try{ cmd.parse( argc, argv ); } 
	catch (TCLAP::ArgException &e)
	{ 
		sprintf(errMsg, "%s for arg %s\n", e.error().c_str(), e.argId().c_str());
		throw iom::exception(errMsg);
	}

	int i;

	/* Checking parameter consistency */
	if ( p_src_format.getValue() != iim::STACKED_FORMAT && 
		 p_src_format.getValue() != iim::SIMPLE_FORMAT  && 
		 p_src_format.getValue() != iim::SIMPLE_RAW_FORMAT  && 
		 p_src_format.getValue() != iim::RAW_FORMAT  && 
		 p_src_format.getValue() != iim::TILED_FORMAT  && 
		 p_src_format.getValue() != iim::TILED_MC_FORMAT &&
		 p_src_format.getValue() != iim::TIF3D_FORMAT  && 
		 p_src_format.getValue() != iim::TILED_TIF3D_FORMAT  && 
		 p_src_format.getValue() != iim::TILED_MC_TIF3D_FORMAT ) {
		temp = "Unknown source format!\nAllowed formats are:\n\t\"" + 
			iim::TILED_MC_FORMAT + "\"/\"" + 
			iim::TILED_FORMAT + "\"/\"" + 
			iim::STACKED_FORMAT + "\"/\"" + 
			iim::SIMPLE_FORMAT + "\"/\"" + 
			iim::SIMPLE_RAW_FORMAT + "\"/\"" + 
			iim::RAW_FORMAT + "\"/\"" + 
			iim::TIF3D_FORMAT + "\"/\"" + 
			iim::TILED_TIF3D_FORMAT  + "\"";
		//sprintf(errMsg, "Unknown source format!\nAllowed formats are:\n\tStacked / Simple / SimpeRaw / Raw / Tiled / TiledMC");
		sprintf(errMsg, "%s", temp.c_str());
		throw iom::exception(errMsg);
	}
	if ( p_dst_format.getValue() != "intensity" && 
		 p_dst_format.getValue() != "graylevel" && 
		 p_dst_format.getValue() != "RGB" ) {
		sprintf(errMsg, "Unknown destination format!\nAllowed formats are:\n\tintensity / graylevel / RGB");
		throw iom::exception(errMsg);
	}
	if ( p_halving_method.getValue() != "mean" && 
		 p_halving_method.getValue() != "max" ) {
		sprintf(errMsg, "Unknown halving method!\nAllowed methods are:\n\tmean / max");
		throw iom::exception(errMsg);
	}
	if ( p_outFmt.getValue() != "Tiff2DStck" && 
		 p_outFmt.getValue() != "Vaa3DRaw"   &&
		 p_outFmt.getValue() != "Tiff3D"   &&
		 p_outFmt.getValue() != "Vaa3DRawMC" &&
		 p_outFmt.getValue() != "Tiff3DMC" ) {
		sprintf(errMsg, "Unknown output format!\nAllowed formats are:\n\tTiff2DStck / Vaa3DRaw / Vaa3DRawMC / Tiff3D / Tiff3DMC");
		throw iom::exception(errMsg);
	}

	//importing parameters not set yet

	this->src_root_dir = p_src_root_dir.getValue();
	this->dst_root_dir = p_dst_root_dir.getValue();
	this->slice_depth = p_slice_depth.getValue();
	this->slice_height = p_slice_height.getValue();
	this->slice_width  = p_slice_width.getValue();
	this->src_format   = p_src_format.getValue();
	this->dst_format   = p_dst_format.getValue();

	this->resolutions[0] = p_highest_resolution.getValue() ? 1 : 0;
	for ( i=1; i<p_n_resolutions.getValue(); i++ )
		this->resolutions[i] = 1;
	for ( ; i<S_MAX_MULTIRES; i++ )
		this->resolutions[i] = 0;

	if ( p_halving_method.getValue() == "mean" )
		this->halving_method = HALVE_BY_MEAN;
	else if ( p_halving_method.getValue() == "max" )
		this->halving_method = HALVE_BY_MAX;

	this->outFmt = p_outFmt.getValue();
}

//checks parameters correctness
void TemplateCLI::checkParams() throw (iom::exception)
{
	//parameters check should be done here.
	//We trust in current tool functions checks.
	//print();

	;
}

//returns help text
string TemplateCLI::getHelpText()
{
	stringstream helptext;
	helptext << "TeraConverter v2.2.0\n";
	return helptext.str();
}

//print all arguments
void TemplateCLI::print()
{
	printf("\n\n");
}
