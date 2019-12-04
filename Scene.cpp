#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <cmath>

#include "Scene.h"
#include "Camera.h"
#include "Color.h"
#include "Model.h"
#include "Rotation.h"
#include "Scaling.h"
#include "Translation.h"
#include "Triangle.h"
#include "Vec3.h"
#include "tinyxml2.h"
#include "Helpers.h"

using namespace tinyxml2;
using namespace std;

/*
	Transformations, clipping, culling, rasterization are done here.
	You can define helper functions inside Scene class implementation.
*/
void Scene::forwardRenderingPipeline(Camera *camera)
{
	// TODO: Implement this function.
}

Matrix4 Scene::translation_matrix(Translation* tr)
{
	Matrix4 tr_matrix = getIdentityMatrix();
	tr_matrix.val[0][3] = tr->tx;
	tr_matrix.val[1][3] = tr->ty;
	tr_matrix.val[2][3] = tr->tz;
	return tr_matrix;			
}

Matrix4 Scene::rotation_matrix(Rotation* rot)
{
	// -1 is a dummy value for color_id field
	Vec3 u(rot->ux, rot->uy, rot->uz, -1);
	u = normalizeVec3(u);

	// create an orthonormal basis 
	// first find absolutely minimum component and make it 0
	int absX = ABS(u.x);
	int absY = ABS(u.y);
	int absZ = ABS(u.z);
	Vec3 v;										//colorId=-1 by default
	if( absX <= absY && absX <= absZ ) 			{v.y=-(u.z); v.z=u.y;}
	else if ( absY <= absX && absY <= absZ ) 	{v.x=-(u.z); v.z=u.x;}
	else 										{v.x=-(u.y); v.y=u.x;}
	//Vec3 v(0, - (u.z), u.y, -1);
	Vec3 w(crossProductVec3(u, v));
	v = normalizeVec3(v);
	w = normalizeVec3(w);

	// M : transforms uvw to xyz
	double M_val[4][4] = {{u.x, u.y, u.z, 0}, 
						  {v.x, v.y, v.z, 0}, 
						  {w.x, w.y, w.z, 0}, 
						  {0, 0, 0, 1}};

	Matrix4 M(M_val);

	std::cout << "M: " << M << std::endl;

	// TODO: write matrix transpose function
	// TODO: write matrix inverse function

	// M_inv transforms xyz to uvw

	double M_inv_val[4][4] = {{u.x, v.x, w.x, 0}, 
							  {u.y, v.y, w.y, 0}, 
							  {u.z, v.z, w.z, 0}, 
							  {0, 0, 0, 1}};

	Matrix4 M_inv(M_inv_val);

	std::cout << "M_inv: " << M_inv << std::endl;

	// R_x is rotation around x (around u after uvw to xyz transformation)

	Matrix4 R_x_matrix = getIdentityMatrix();
	double radian = (rot->angle * M_PI) / 180.0;
	R_x_matrix.val[1][1] = cos(radian);
	R_x_matrix.val[1][2] = -sin(radian);
	R_x_matrix.val[2][1] = sin(radian);
	R_x_matrix.val[2][2] = cos(radian);

	std::cout << "R_x: " << R_x_matrix << std::endl;

	// M_inv * R_x * M
	Matrix4 final_matrix(multiplyMatrixWithMatrix(multiplyMatrixWithMatrix(M_inv, R_x_matrix), M));

	return final_matrix;

}

Matrix4 Scene::scaling_matrix(Scaling* sc)
{
	Matrix4 sc_matrix = getIdentityMatrix();
	sc_matrix.val[0][0] = sc->sx;
	sc_matrix.val[1][1] = sc->sy;
	sc_matrix.val[2][2] = sc->sz;

	return sc_matrix;
}

Matrix4 Scene::transformation_matrix_of_model(Model* model)
{
	Matrix4 final_matrix(getIdentityMatrix());

	for (int i = 0; i < model->numberOfTransformations; i++)
	{
		if (model->transformationTypes[i] == 't')
		{
			Matrix4 tr_m = Scene::translation_matrix(translations[model->transformationIds[i] - 1]);
			final_matrix = multiplyMatrixWithMatrix(tr_m, final_matrix);
		}
		else if (model->transformationTypes[i] == 's')
		{
			Matrix4 sc_m = Scene::scaling_matrix(scalings[model->transformationIds[i] - 1]);
			final_matrix = multiplyMatrixWithMatrix(sc_m, final_matrix);
		}
		else if (model->transformationTypes[i] == 'r')
		{
			Matrix4 rot_m = Scene::rotation_matrix(rotations[model->transformationIds[i] - 1]);
			final_matrix = multiplyMatrixWithMatrix(rot_m, final_matrix);
		}
	}

	return final_matrix;
}

// Triangle return etmesi gerekmiyor mu? Ya da void yapmamız gerekmiyor mu?
Triangle Scene::transform_triangle(Triangle triangle, Matrix4 tf_matrix)
{
	Vec3* v1_vec3 = vertices[triangle.getFirstVertexId() - 1];
	Vec3* v2_vec3 = vertices[triangle.getSecondVertexId() - 1];
	Vec3* v3_vec3 = vertices[triangle.getThirdVertexId() - 1];
	Vec4 v1(v1_vec3->x, v1_vec3->y, v1_vec3->z, 1, -1);
	Vec4 v2(v2_vec3->x, v2_vec3->y, v2_vec3->z, 1, -1);
	Vec4 v3(v3_vec3->x, v3_vec3->y, v3_vec3->z, 1, -1); 

	v1 = multiplyMatrixWithVec4(tf_matrix, v1);
	v2 = multiplyMatrixWithVec4(tf_matrix, v2);
	v3 = multiplyMatrixWithVec4(tf_matrix, v3);
}

Matrix4 Scene::camera_transformation(Camera* camera){
	Matrix4 tr_matrix = getIdentityMatrix();
	tr_matrix.val[0][3] = -camera->pos.x;
	tr_matrix.val[1][3] = -camera->pos.y;
	tr_matrix.val[2][3] = -camera->pos.z;

	double rotateUVW[4][4] = {{camera->u.x, camera->u.y, camera->u.z, 0}, 
							  {camera->v.x, camera->v.y, camera->v.z, 0}, 
							  {camera->w.x, camera->w.y, camera->w.z, 0}, 
							  {0, 0, 0, 1}};
	Matrix4 rotate_uvw(rotateUVW);

	Matrix4 M_cam= multiplyMatrixWithMatrix(rotate_uvw,tr_matrix);

	return M_cam;	
}

// projection_type==0 means orthographic transformation, ==1 means perspective projection
Matrix4 Scene::projection_transformation(Camera* camera,int projection_type){
	double l = camera->left;
	double r = camera->right;
	double t = camera->top;
	double b = camera->bottom;
	double f = camera->far;
	double n = camera->near;
	if (projection_type == 0){
		double M_orthographic[4][4] = {{2/(r-l), 0, 0, -(r+l)/(r-l)}, 
							  	{0, 2/(t-b), 0, -(t+b)/(t-b)}, 
							  	{0, 0, -2/(f-n), -(f+n)/(f-n)}, 
							  	{0, 0, 0, 1}};
		Matrix4 M_orth(M_orthographic);
		return M_orth;
	}
	else if (projection_type == 1){
		double M_perspective[4][4] = {	{(2*n)/(r-l), 0, (r+l)/(r-l), 0}, 
							  			{0, (2*n)/(t-b), (t+b)/(t-b), 0}, 
							  			{0, 0, -(f+n)/(f-n), (-2*f*n)/(f-n)}, 
							  			{0, 0, -1, }};
		Matrix4 M_orth(M_perspective);
		return M_orth;
	}
}


/*
	Parses XML file
*/
Scene::Scene(const char *xmlPath)
{
	const char *str;
	XMLDocument xmlDoc;
	XMLElement *pElement;

	xmlDoc.LoadFile(xmlPath);

	XMLNode *pRoot = xmlDoc.FirstChild();

	// read background color
	pElement = pRoot->FirstChildElement("BackgroundColor");
	str = pElement->GetText();
	sscanf(str, "%lf %lf %lf", &backgroundColor.r, &backgroundColor.g, &backgroundColor.b);

	// read culling
	pElement = pRoot->FirstChildElement("Culling");
	if (pElement != NULL)
		pElement->QueryBoolText(&cullingEnabled);

	// read projection type
	pElement = pRoot->FirstChildElement("ProjectionType");
	if (pElement != NULL)
		pElement->QueryIntText(&projectionType);

	// read cameras
	pElement = pRoot->FirstChildElement("Cameras");
	XMLElement *pCamera = pElement->FirstChildElement("Camera");
	XMLElement *camElement;
	while (pCamera != NULL)
	{
		Camera *cam = new Camera();

		pCamera->QueryIntAttribute("id", &cam->cameraId);

		camElement = pCamera->FirstChildElement("Position");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->pos.x, &cam->pos.y, &cam->pos.z);

		camElement = pCamera->FirstChildElement("Gaze");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->gaze.x, &cam->gaze.y, &cam->gaze.z);

		camElement = pCamera->FirstChildElement("Up");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->v.x, &cam->v.y, &cam->v.z);

		cam->gaze = normalizeVec3(cam->gaze);
		cam->u = crossProductVec3(cam->gaze, cam->v);
		cam->u = normalizeVec3(cam->u);

		cam->w = inverseVec3(cam->gaze);
		cam->v = crossProductVec3(cam->u, cam->gaze);
		cam->v = normalizeVec3(cam->v);

		camElement = pCamera->FirstChildElement("ImagePlane");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf %lf %lf %lf %d %d",
			   &cam->left, &cam->right, &cam->bottom, &cam->top,
			   &cam->near, &cam->far, &cam->horRes, &cam->verRes);

		camElement = pCamera->FirstChildElement("OutputName");
		str = camElement->GetText();
		cam->outputFileName = string(str);

		cameras.push_back(cam);

		pCamera = pCamera->NextSiblingElement("Camera");
	}

	// read vertices
	pElement = pRoot->FirstChildElement("Vertices");
	XMLElement *pVertex = pElement->FirstChildElement("Vertex");
	int vertexId = 1;

	while (pVertex != NULL)
	{
		Vec3 *vertex = new Vec3();
		Color *color = new Color();

		vertex->colorId = vertexId;

		str = pVertex->Attribute("position");
		sscanf(str, "%lf %lf %lf", &vertex->x, &vertex->y, &vertex->z);

		str = pVertex->Attribute("color");
		sscanf(str, "%lf %lf %lf", &color->r, &color->g, &color->b);

		vertices.push_back(vertex);
		colorsOfVertices.push_back(color);

		pVertex = pVertex->NextSiblingElement("Vertex");

		vertexId++;
	}

	// read translations
	pElement = pRoot->FirstChildElement("Translations");
	XMLElement *pTranslation = pElement->FirstChildElement("Translation");
	while (pTranslation != NULL)
	{
		Translation *translation = new Translation();

		pTranslation->QueryIntAttribute("id", &translation->translationId);

		str = pTranslation->Attribute("value");
		sscanf(str, "%lf %lf %lf", &translation->tx, &translation->ty, &translation->tz);

		translations.push_back(translation);

		pTranslation = pTranslation->NextSiblingElement("Translation");
	}

	// read scalings
	pElement = pRoot->FirstChildElement("Scalings");
	XMLElement *pScaling = pElement->FirstChildElement("Scaling");
	while (pScaling != NULL)
	{
		Scaling *scaling = new Scaling();

		pScaling->QueryIntAttribute("id", &scaling->scalingId);
		str = pScaling->Attribute("value");
		sscanf(str, "%lf %lf %lf", &scaling->sx, &scaling->sy, &scaling->sz);

		scalings.push_back(scaling);

		pScaling = pScaling->NextSiblingElement("Scaling");
	}

	// read rotations
	pElement = pRoot->FirstChildElement("Rotations");
	XMLElement *pRotation = pElement->FirstChildElement("Rotation");
	while (pRotation != NULL)
	{
		Rotation *rotation = new Rotation();

		pRotation->QueryIntAttribute("id", &rotation->rotationId);
		str = pRotation->Attribute("value");
		sscanf(str, "%lf %lf %lf %lf", &rotation->angle, &rotation->ux, &rotation->uy, &rotation->uz);

		rotations.push_back(rotation);

		pRotation = pRotation->NextSiblingElement("Rotation");
	}

	// read models
	pElement = pRoot->FirstChildElement("Models");

	XMLElement *pModel = pElement->FirstChildElement("Model");
	XMLElement *modelElement;
	while (pModel != NULL)
	{
		Model *model = new Model();

		pModel->QueryIntAttribute("id", &model->modelId);
		pModel->QueryIntAttribute("type", &model->type);

		// read model transformations
		XMLElement *pTransformations = pModel->FirstChildElement("Transformations");
		XMLElement *pTransformation = pTransformations->FirstChildElement("Transformation");

		pTransformations->QueryIntAttribute("count", &model->numberOfTransformations);

		while (pTransformation != NULL)
		{
			char transformationType;
			int transformationId;

			str = pTransformation->GetText();
			sscanf(str, "%c %d", &transformationType, &transformationId);

			model->transformationTypes.push_back(transformationType);
			model->transformationIds.push_back(transformationId);

			pTransformation = pTransformation->NextSiblingElement("Transformation");
		}

		// read model triangles
		XMLElement *pTriangles = pModel->FirstChildElement("Triangles");
		XMLElement *pTriangle = pTriangles->FirstChildElement("Triangle");

		pTriangles->QueryIntAttribute("count", &model->numberOfTriangles);

		while (pTriangle != NULL)
		{
			int v1, v2, v3;

			str = pTriangle->GetText();
			sscanf(str, "%d %d %d", &v1, &v2, &v3);

			model->triangles.push_back(Triangle(v1, v2, v3));

			pTriangle = pTriangle->NextSiblingElement("Triangle");
		}

		models.push_back(model);

		pModel = pModel->NextSiblingElement("Model");
	}
}

/*
	Initializes image with background color
*/
void Scene::initializeImage(Camera *camera)
{
	if (this->image.empty())
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			vector<Color> rowOfColors;

			for (int j = 0; j < camera->verRes; j++)
			{
				rowOfColors.push_back(this->backgroundColor);
			}

			this->image.push_back(rowOfColors);
		}
	}
	// if image is filled before, just change color rgb values with the background color
	else
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			for (int j = 0; j < camera->verRes; j++)
			{
				this->image[i][j].r = this->backgroundColor.r;
				this->image[i][j].g = this->backgroundColor.g;
				this->image[i][j].b = this->backgroundColor.b;
			}
		}
	}
}

/*
	If given value is less than 0, converts value to 0.
	If given value is more than 255, converts value to 255.
	Otherwise returns value itself.
*/
int Scene::makeBetweenZeroAnd255(double value)
{
	if (value >= 255.0)
		return 255;
	if (value <= 0.0)
		return 0;
	return (int)(value);
}

/*
	Writes contents of image (Color**) into a PPM file.
*/
void Scene::writeImageToPPMFile(Camera *camera)
{
	ofstream fout;

	fout.open(camera->outputFileName.c_str());

	fout << "P3" << endl;
	fout << "# " << camera->outputFileName << endl;
	fout << camera->horRes << " " << camera->verRes << endl;
	fout << "255" << endl;

	for (int j = camera->verRes - 1; j >= 0; j--)
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			fout << makeBetweenZeroAnd255(this->image[i][j].r) << " "
				 << makeBetweenZeroAnd255(this->image[i][j].g) << " "
				 << makeBetweenZeroAnd255(this->image[i][j].b) << " ";
		}
		fout << endl;
	}
	fout.close();
}

/*
	Converts PPM image in given path to PNG file, by calling ImageMagick's 'convert' command.
	os_type == 1 		-> Ubuntu
	os_type == 2 		-> Windows
	os_type == other	-> No conversion
*/
void Scene::convertPPMToPNG(string ppmFileName, int osType)
{
	string command;

	// call command on Ubuntu
	if (osType == 1)
	{
		command = "convert " + ppmFileName + " " + ppmFileName + ".png";
		system(command.c_str());
	}

	// call command on Windows
	else if (osType == 2)
	{
		command = "magick convert " + ppmFileName + " " + ppmFileName + ".png";
		system(command.c_str());
	}

	// default action - don't do conversion
	else
	{
	}
}