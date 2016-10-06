#include <iostream>
using namespace std;

void initLighting()
{
	GLfloat amb_light[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat diffuse[] = { 0.6, 0.6, 0.6, 1 };
    GLfloat specular[] = { 0.7, 0.7, 0.3, 1 };
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, amb_light );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse );
    glLightfv( GL_LIGHT0, GL_SPECULAR, specular );
    glEnable( GL_LIGHT0 );
    glEnable( GL_COLOR_MATERIAL );
    glShadeModel( GL_SMOOTH );
    glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE );
    glDepthFunc( GL_LEQUAL );
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0); 
}

class OBJParsing
{
  public: 
	OBJParsing();			
    float* calculateNormal(float* coord1,float* coord2,float* coord3 );
    int Load(const char *filename);	// Loads the model
	void Draw();					// Draws the model on the screen
	void Release();				// Release the model
 
	float* normals;							// Stores the normals
    float* Faces_Triangles;					// Stores the triangles
	float* vertexBuffer;					// Stores the points which make the object
	long TotalConnectedPoints;				// Stores the total number of connected verteces
	long TotalConnectedTriangles;			// Stores the total number of connected triangles
 
};
 
 
#define POINTS_PER_VERTEX 3
#define TOTAL_FLOATS_IN_TRIANGLE 9
 
OBJParsing::OBJParsing()
{
	this->TotalConnectedTriangles = 0; 
	this->TotalConnectedPoints = 0;
}
 
float* OBJParsing::calculateNormal( float *coord1, float *coord2, float *coord3 )
{
   /* calculate Vector1 and Vector2 */
   float va[3], vb[3], vr[3], val;
   va[0] = coord1[0] - coord2[0];
   va[1] = coord1[1] - coord2[1];
   va[2] = coord1[2] - coord2[2];
 
   vb[0] = coord1[0] - coord3[0];
   vb[1] = coord1[1] - coord3[1];
   vb[2] = coord1[2] - coord3[2];
 
   /* cross product */
   vr[0] = va[1] * vb[2] - vb[1] * va[2];
   vr[1] = vb[0] * va[2] - va[0] * vb[2];
   vr[2] = va[0] * vb[1] - vb[0] * va[1];
 
    /* normalization factor */
    val = sqrt(vr[0]*vr[0] + vr[1]*vr[1] + vr[2]*vr[2]);
 
	float *norm = new float[3];
	norm[0] = vr[0]/val;
	norm[1] = vr[1]/val;
	norm[2] = vr[2]/val;
  
	return norm;
}
 
 
int OBJParsing::Load(const char* filename)
{
	string line;
	ifstream objFile (filename);	
	if (objFile.is_open())													// If obj file is open, continue
	{
		objFile.seekg (0, ios::end);										// Go to end of the file, 
		long fileSize = objFile.tellg();									// get file size
		objFile.seekg (0, ios::beg);										// we'll use this to register memory for our 3d model
 
		vertexBuffer = (float*) malloc (fileSize);							// Allocate memory for the verteces
		Faces_Triangles = (float*) malloc(fileSize*sizeof(float));			// Allocate memory for the triangles
		normals  = (float*) malloc(fileSize*sizeof(float));					// Allocate memory for the normals
 
		int triangle_index = 0;												// Set triangle index to zero
		int normal_index = 0;												// Set normal index to zero
 
		while (! objFile.eof() )											// Start reading file data
		{		
			getline (objFile,line);											// Get line from file
 
			if (line.c_str()[0] == 'v')										// The first character is a v: on this line is a vertex stored.
			{
				line[0] = ' ';												// Set first character to 0. This will allow us to use sscanf
 
				sscanf(line.c_str(),"%f %f %f ",							// Read floats from the line: v X Y Z
					&vertexBuffer[TotalConnectedPoints],
					&vertexBuffer[TotalConnectedPoints+1], 
					&vertexBuffer[TotalConnectedPoints+2]);
 
				TotalConnectedPoints += POINTS_PER_VERTEX;					// Add 3 to the total connected points
			}
			if (line.c_str()[0] == 'f')										// The first character is an 'f': on this line is a point stored
			{
		    	line[0] = ' ';												// Set first character to 0. This will allow us to use sscanf
 				int temp;
				int vertexNumber[4] = { 0, 0, 0 };
                
                sscanf(line.c_str(),"%i//%i%i//%i%i//%i",								// Read integers from the line:  f 1 2 3
					&vertexNumber[0],	&temp,									// First point of our triangle. This is an 
					&vertexNumber[1],	&temp,									// pointer to our vertexBuffer list
					&vertexNumber[2],	&temp );

                sscanf(line.c_str(),"%i/%i/%i%i/%i/%i%i/%i/%i",								// Read integers from the line:  f 1 2 3
					&vertexNumber[0],	&temp,	&temp,								// First point of our triangle. This is an 
					&vertexNumber[1],	&temp,	&temp,								// pointer to our vertexBuffer list
					&vertexNumber[2],	&temp,	&temp);

                sscanf(line.c_str(),"%i/%i%i/%i%i/%i",								// Read integers from the line:  f 1 2 3
					&vertexNumber[0],	&temp,									// First point of our triangle. This is an 
					&vertexNumber[1],	&temp,									// pointer to our vertexBuffer list
					&vertexNumber[2],	&temp );
				
				sscanf(line.c_str(),"%i%i%i",								// Read integers from the line:  f 1 2 3
					&vertexNumber[0],										// First point of our triangle. This is an 
					&vertexNumber[1],										// pointer to our vertexBuffer list
					&vertexNumber[2]);										// each point represents an X,Y,Z.
 				
				vertexNumber[0] -= 1;										// OBJ file starts counting from 1
				vertexNumber[1] -= 1;										// OBJ file starts counting from 1
				vertexNumber[2] -= 1;										// OBJ file starts counting from 1
 
 
				/********************************************************************
				 * Create triangles (f 1 2 3) from points: (v X Y Z) (v X Y Z) (v X Y Z). 
				 * The vertexBuffer contains all verteces
				 * The triangles will be created using the verteces we read previously
				 */
 
				int tCounter = 0;
				for (int i = 0; i < POINTS_PER_VERTEX; i++)					
				{
					Faces_Triangles[triangle_index + tCounter   ] = vertexBuffer[3*vertexNumber[i] ];
					Faces_Triangles[triangle_index + tCounter +1 ] = vertexBuffer[3*vertexNumber[i]+1 ];
					Faces_Triangles[triangle_index + tCounter +2 ] = vertexBuffer[3*vertexNumber[i]+2 ];
					tCounter += POINTS_PER_VERTEX;
				}
 
				/*********************************************************************
				 * Calculate all normals, used for lighting
				 */ 
				float coord1[3] = { Faces_Triangles[triangle_index], Faces_Triangles[triangle_index+1],Faces_Triangles[triangle_index+2]};
				float coord2[3] = {Faces_Triangles[triangle_index+3],Faces_Triangles[triangle_index+4],Faces_Triangles[triangle_index+5]};
				float coord3[3] = {Faces_Triangles[triangle_index+6],Faces_Triangles[triangle_index+7],Faces_Triangles[triangle_index+8]};
				float *norm = this->calculateNormal( coord1, coord2, coord3 );
 
				tCounter = 0;
				for (int i = 0; i < POINTS_PER_VERTEX; i++)
				{
					normals[normal_index + tCounter ] = norm[0];
					normals[normal_index + tCounter +1] = norm[1];
					normals[normal_index + tCounter +2] = norm[2];
					tCounter += POINTS_PER_VERTEX;
				}
 
				triangle_index += TOTAL_FLOATS_IN_TRIANGLE;
				normal_index += TOTAL_FLOATS_IN_TRIANGLE;
				TotalConnectedTriangles += TOTAL_FLOATS_IN_TRIANGLE;			
			}	
		}
		objFile.close();														// Close OBJ file
	}
	else 
	{
		cout << "Unable to open file";								
	}
	return 0;
}
 
void OBJParsing::Release()
{
	free(this->Faces_Triangles);
	free(this->normals);
	free(this->vertexBuffer);
}
 
void OBJParsing::Draw()
{
	
	glEnableClientState(GL_VERTEX_ARRAY);						// Enable vertex arrays
 	glEnableClientState(GL_NORMAL_ARRAY);						// Enable normal arrays
	glVertexPointer(3,GL_FLOAT,	0,Faces_Triangles);				// Vertex Pointer to triangle array
	glNormalPointer(GL_FLOAT, 0, normals);						// Normal pointer to normal array
	glDrawArrays(GL_TRIANGLES, 0, TotalConnectedTriangles);		// Draw the triangles
	glDisableClientState(GL_VERTEX_ARRAY);						// Disable vertex arrays
	glDisableClientState(GL_NORMAL_ARRAY);						// Disable normal arrays

}
