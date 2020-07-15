#include "reduceCmd.h"

#include <maya/MFnPlugin.h>

MStatus initializePlugin(MObject obj) {

	MFnPlugin fnPlugin(obj, "Xingyu", "0.1", "2018");

	fnPlugin.registerCommand("reduceCmd",
		ReduceCmd::creater,
		ReduceCmd::newSyntax);

	return MS::kSuccess;
}

MStatus uninitializePlugin(MObject obj) {

	MFnPlugin fnPlugin(obj);
	fnPlugin.deregisterCommand("reduceCmd");

	return MS::kSuccess;
}