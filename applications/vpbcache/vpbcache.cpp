/* -*-c++-*- VirtualPlanetBuilder - Copyright (C) 1998-2007 Robert Osfield 
 *
 * This application is open source and may be redistributed and/or modified   
 * freely and without restriction, both in commericial and non commericial applications,
 * as long as this copyright notice is maintained.
 * 
 * This application is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/


#include <vpb/Commandline>
#include <vpb/TaskManager>
#include <vpb/BuildLog>
#include <vpb/System>

#include <osg/Timer>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <iostream>

int main(int argc, char** argv)
{
    osg::ArgumentParser arguments(&argc,argv);

    // set up the usage document, in case we need to print out how to use this program.
    arguments.getApplicationUsage()->setApplicationName(arguments.getApplicationName());
    arguments.getApplicationUsage()->setDescription(arguments.getApplicationName()+" application is utility tools which can be used to generate paged geospatial terrain databases.");
    arguments.getApplicationUsage()->setCommandLineUsage(arguments.getApplicationName()+" [options] filename ...");
    arguments.getApplicationUsage()->addCommandLineOption("-h or --help","Display this information");
    arguments.getApplicationUsage()->addCommandLineOption("--clean","Clear the contents of the file cache");
    arguments.getApplicationUsage()->addCommandLineOption("--cache","Specify cache file to use");
    arguments.getApplicationUsage()->addCommandLineOption("--reproject","Carry out reprojections required for specified build sources.");
    arguments.getApplicationUsage()->addCommandLineOption("--add","Add files from specified build sources to file cache.");
    arguments.getApplicationUsage()->addCommandLineOption("--overviews","Build overviews for the source data.");
    arguments.getApplicationUsage()->addCommandLineOption("--report","Report the contents of the file cache");


    // if user request help write it out to cout.
    if (arguments.read("-h") || arguments.read("--help"))
    {
        arguments.getApplicationUsage()->write(std::cout,osg::ApplicationUsage::COMMAND_LINE_OPTION);
        return 1;
    }

    // read any source input definitions
    osg::ref_ptr<osgTerrain::Terrain> terrain = new osgTerrain::Terrain;

    std::string filename;
    if (arguments.read("-s",filename))
    {
        osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(filename);
        if (node.valid())
        {
            osgTerrain::Terrain* loaded_terrain = dynamic_cast<osgTerrain::Terrain*>(node.get());
            if (loaded_terrain) 
            {
                terrain = loaded_terrain;
            }
            else
            {
                vpb::log(osg::WARN,"Error: source file \"%s\" not suitable terrain data.",filename.c_str());
                return 1;
            }
        }
        else
        {
            vpb::log(osg::WARN,"Error: unable to load source file \"%s\" not suitable terrain data.",filename.c_str());
            return 1;
        }
        
    }
    
    int result = vpb::readSourceArguments(std::cout, arguments, terrain.get());
    if (result) return result;
    
    vpb::System::instance()->readArguments(arguments);

    vpb::FileCache* fileCache = vpb::System::instance()->getFileCache();
    if (!fileCache)
    {
        vpb::log(osg::WARN,"Error: no valid cache file specificed, please set one using --cache <filename> on command line.");
        return 1;
    }

    if (arguments.read("--clear"))
    {
        fileCache->clear();
    }

    if (arguments.read("--add"))
    {
        fileCache->addSource(terrain.get());
    }

    if (arguments.read("--reproject"))
    {
        fileCache->buildRequiredReprojections(terrain.get());
    }

    if (arguments.read("--overviews"))
    {
        fileCache->buildOverviews(terrain.get());
    }

    std::string machineName;
    while(arguments.read("--mirror", machineName))
    {
        if (!vpb::System::instance()->getMachinePool())
        {
            vpb::log(osg::WARN,"Error: no valid machines file specified, please set one using --machines <filename> on command line.");
            return 1;
        }
    
        vpb::Machine* machine = vpb::System::instance()->getMachinePool()->getMachine(machineName);
        if (machine)
        {
            fileCache->mirror(machine, terrain.get());
        }
        else
        {
            osg::notify(osg::NOTICE)<<"No suitable machine found"<<std::endl;
        }
    }

    fileCache->sync();

    if (arguments.read("--report"))
    {
        fileCache->report(std::cout);
    }


    // any option left unread are converted into errors to write out later.
    arguments.reportRemainingOptionsAsUnrecognized();

    // report any errors if they have occured when parsing the program aguments.
    if (arguments.errors())
    {
        arguments.writeErrorMessages(std::cout);
        return 1;
    }

    return 0;
}
