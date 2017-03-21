// FBXSDKTest1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fbxsdk.h>

int numOfTabs = 0;

void PrintTabs() {
	for (int i = 0; i < numOfTabs; i++) {
		printf("\t");
	}
}

FbxString GetAttributeTypeName(FbxNodeAttribute::EType type) {
	switch (type)
	{
	case fbxsdk::FbxNodeAttribute::eUnknown:
		return "unknown";
	case fbxsdk::FbxNodeAttribute::eNull:
		return "null";
	case fbxsdk::FbxNodeAttribute::eMarker:
		return "marker";
	case fbxsdk::FbxNodeAttribute::eSkeleton:
		return "skeleton";
	case fbxsdk::FbxNodeAttribute::eMesh:
		return "mesh";
	case fbxsdk::FbxNodeAttribute::eNurbs:
		return "nurbs";
	case fbxsdk::FbxNodeAttribute::ePatch:
		return "patch";
	case fbxsdk::FbxNodeAttribute::eCamera:
		return "camera";
	case fbxsdk::FbxNodeAttribute::eCameraStereo:
		return "stereo";
	case fbxsdk::FbxNodeAttribute::eCameraSwitcher:
		return "camera switcher";
	case fbxsdk::FbxNodeAttribute::eLight:
		return "light";
	case fbxsdk::FbxNodeAttribute::eOpticalReference:
		return "optical reference";
	case fbxsdk::FbxNodeAttribute::eOpticalMarker:
		return "marker";
	case fbxsdk::FbxNodeAttribute::eNurbsCurve:
		return "nurbs curve";
	case fbxsdk::FbxNodeAttribute::eTrimNurbsSurface:
		return "trim nurbs surface";
	case fbxsdk::FbxNodeAttribute::eBoundary:
		return "boundary";
	case fbxsdk::FbxNodeAttribute::eNurbsSurface:
		return "nurbs surface";
	case fbxsdk::FbxNodeAttribute::eShape:
		return "shape";
	case fbxsdk::FbxNodeAttribute::eLODGroup:
		return "lodgroup";
	case fbxsdk::FbxNodeAttribute::eSubDiv:
		return "subdiv";
	case fbxsdk::FbxNodeAttribute::eCachedEffect:
		return "cached effect";
	case fbxsdk::FbxNodeAttribute::eLine:
		return "line";
	default:
		return "unknown";
	}
}

void PrintAttribute(FbxNodeAttribute *pAttribute) {
	if (!pAttribute)
		return;

	FbxString typeName = GetAttributeTypeName(pAttribute->GetAttributeType());
	FbxString attrName = pAttribute->GetName();

	PrintTabs();

	printf("<attribute type='%s' name='%s' />\n", typeName.Buffer(), attrName.Buffer());
}

void PrintNode(FbxNode *pNode) {
	PrintTabs();

	const char *lNodeName = pNode->GetName();

	FbxDouble3 translation = pNode->LclTranslation.Get();

	FbxDouble3 rotaion = pNode->LclRotation.Get();

	FbxDouble3 scaling = pNode->LclScaling.Get();

	printf("<node name='%s' translation='(%f, %f, %f)' rotation='(%f, %f, %f)' scaling='(%f, %f, %f)'>\n",
		lNodeName,
		translation[0], translation[1], translation[2],
		rotaion[0], rotaion[1], rotaion[2],
		scaling[0], scaling[1], scaling[2]);

	numOfTabs++;

	for (int i = 0; i < pNode->GetNodeAttributeCount(); i++) {
		PrintAttribute(pNode->GetNodeAttributeByIndex(i));
	}

	for (int j = 0; j < pNode->GetChildCount(); j++) {
		PrintNode(pNode->GetChild(j));
	}

	numOfTabs--;
	PrintTabs();
	printf("</node>\n");

}

int _tmain(int argc, _TCHAR* argv[])
{
	printf("Setup Done!\n");

	const char *lFilename = "F-15C_Eagle.fbx";

	FbxManager *lSdkManager = FbxManager::Create();

	FbxIOSettings *lioSettings = FbxIOSettings::Create(lSdkManager, IOSROOT);
	
	lSdkManager->SetIOSettings(lioSettings);

	FbxImporter *lImporter = FbxImporter::Create(lSdkManager, "");

	if (!lImporter->Initialize(lFilename, -1, lSdkManager->GetIOSettings())) {
		printf("Call to FbxImporter->Initialize() failed.\n");
		printf("Error returned: %s \n\n", lImporter->GetStatus().GetErrorString());
		exit(-1);
	}

	FbxScene *lScene = FbxScene::Create(lSdkManager, "sceneName");

	lImporter->Import(lScene);

	lImporter->Destroy();


	FbxNode *lRootNode = lScene->GetRootNode();

	if (lRootNode) {
		for (int i = 0; i < lRootNode->GetChildCount(); i++) {
			PrintNode(lRootNode->GetChild(i));
		}
	}

	lSdkManager->Destroy();


	system("pause");
	return 0;
}