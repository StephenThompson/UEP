#include <iostream>
#include <string>

#include "mainheaders.hpp"

#include "preystrategy.hpp"
#include "predatorstrategy.hpp"
#include "scenemanager.hpp"
#include "imageLoader.hpp"
#include "shaderLoader.hpp"

#include "projection.hpp"
#include "voronoi.hpp"

#include "mainheaders.hpp"
#include "coral.hpp"
#include "SpaceCoral.hpp"

using namespace std;
using namespace glm;

// Stephen
SceneManager *sceneMng = nullptr;
Geometry* model = nullptr;
Geometry* modelScissortail = nullptr;
Geometry* modelDolphin = nullptr;
Geometry* modelHerring = nullptr;
Geometry* modelSpongeBob = nullptr;

GLuint g_texBannerfish = 0;
GLuint g_texScissortail = 0;
GLuint g_texdolphin = 0;
GLuint g_texHerring = 0;
GLuint g_texSpongeBob = 0;
GLuint g_shaderFish = 0;

// mouse controls
bool g_mouseDown = false;
vec2 g_mousePos;
vec3 g_cameraPos = vec3(0,-200,0);
float g_yRotation = 0;
float g_xRotation = 0;
float g_zoomFactor = 1.0;
bool g_simulate = true;

// Cullum
Projection *g_projection = nullptr;
Voronoi *g_voroni = nullptr;

GLuint sand_texture = 0;
GLuint cube_map_texture = 0;
GLuint texture = 0;
GLuint g_shader = 0;
GLuint g_shaderSurface = 0;
GLuint g_shaderCoral = 0;
float shift = 0;

void initTexture(GLuint *texptr, string file) {
	glGenTextures(1, texptr); // Generate texture ID
	glBindTexture(GL_TEXTURE_2D, *texptr); // Bind it as a 2D texture

	// Setup sampling strategies
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Finnaly, actually fill the data into our texture
	image tex(file);
	//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, tex.w, tex.h, tex.glFormat(), GL_UNSIGNED_BYTE, tex.dataPointer());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.w, tex.h, 0, tex.glFormat(), GL_UNSIGNED_BYTE, tex.dataPointer());
	glGenerateMipmap(GL_TEXTURE_2D);
}

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_B && action == GLFW_RELEASE)
		sceneMng->toggleDebug();

	if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
		g_simulate = !g_simulate;

	if (key == GLFW_KEY_W)
		g_cameraPos -= vec3(cos(radians(g_yRotation)), sin(radians(g_xRotation)), sin(radians(g_yRotation))) * 5.f;
	if (key == GLFW_KEY_S)
		g_cameraPos += vec3(cos(radians(g_yRotation)), sin(radians(g_xRotation)), sin(radians(g_yRotation))) * 5.f;
	if (key == GLFW_KEY_A)
		g_cameraPos += vec3(cos(radians(g_yRotation+90)), 0, sin(radians(g_yRotation+90))) * 5.f;
	if (key == GLFW_KEY_D)
		g_cameraPos -= vec3(cos(radians(g_yRotation+90)), 0, sin(radians(g_yRotation+90))) * 5.f;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
	//cout << "Mouse Callback :: button=" << button << ", state=" << state << ", (" << x << "," << y << ")" << endl;
	if (button == GLFW_MOUSE_BUTTON_LEFT){ //left mouse button
		g_mouseDown = (action == GLFW_PRESS);
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
	if (yoffset > 0){
		//scroll foward/up
		g_zoomFactor /= 1.1f;
	}
	else if (yoffset < 0){
		//scroll back/down
		g_zoomFactor *= 1.1f;
	}
}

void cursor_pos_callback(GLFWwindow* window, double x, double y) {
	//cout << "Mouse Motion Callback :: (" << x << "," << y << ")" << endl;
	if (g_mouseDown) {
		vec2 dif = vec2(x, y) - g_mousePos;
		g_yRotation += 0.3f * dif.x;
		g_xRotation += 0.3f * dif.y;
	}
	g_mousePos = vec2(x, y);
}


float noise2D(int x,int y){
	int n = x+y*50;
	n = (n<<13)^n;
	return ( 1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

void sandTexture(){
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE1);
	int w = 2000;
	int h = 2000;

	float* data = new float[w*h*3];

	float tone = 0.9;
	glPointSize(1);
	for (int x = 0; x < w; x++){
		for (int y = 0;y < h;y++){
			float genValue = (noise2D(x,y+3000)+1)/2;
			float r = tone*((((244-108)*genValue)+108)/255);
			float g = tone*((((201-84)*genValue)+84)/255);
			float b = tone*((((175-30)*genValue)+30)/255);

			data[(x * w * 3 + y*3) + 0] = r;
			data[(x * w * 3 + y*3) + 1] = g;
			data[(x * w * 3 + y*3) + 2] = b;
		}
	}
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1,&sand_texture);
	glBindTexture(GL_TEXTURE_2D, sand_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0 ,GL_RGB, w,h,0,GL_RGB,GL_FLOAT,data);


	for (int x = 0; x < w; x++){
		for (int y = 0;y < h;y++){


			data[(x * w * 3 + y*3) + 0] = 0.8;
			data[(x * w * 3 + y*3) + 1] = 0.75;
			data[(x * w * 3 + y*3) + 2] = 0.55;
		}
	}


	glActiveTexture(GL_TEXTURE2);

	glGenTextures(1,&texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0 ,GL_RGB, w,h,0,GL_RGB,GL_FLOAT,data);

}

void initShader() {
	g_shaderFish = comp308::makeShaderProgram("../work/res/shaders/shaderFish.vert", "../work/res/shaders/shaderFish.frag");
	g_shader = comp308::makeShaderProgram("../work/res/shaders/shaderDemo.vert", "../work/res/shaders/shaderDemo.frag");
	g_shaderSurface = comp308::makeShaderProgram("../work/res/shaders/shaderSurface.vert", "../work/res/shaders/shaderSurface.frag");
	g_shaderCoral = comp308::makeShaderProgram("../work/res/shaders/shaderCoral.vert", "../work/res/shaders/shaderCoral.frag");
}

void initLight() {
	float direction[]	  = {0.7f, 1.0f, 0.5f, 0.0f};
	float diffintensity[] = {0.7f, 0.7f, 0.7f, 1.0f};
	float ambient[]       = {0.5f, 0.5f, 0.5f, 1.0f};

	glLightfv(GL_LIGHT0, GL_POSITION, direction);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffintensity);
	glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);


	glEnable(GL_LIGHT0);
}

void setupShader(){
	vec3 lightInvDir = vec3(0.5f,2,2);

	// Compute the MVP matrix from the light's point of view
	mat4 depthProjectionMatrix = ortho<float>(-10,10,-10,10,-10,10);
	mat4 depthViewMatrix = lookAt(lightInvDir, vec3(0,0,0), vec3(0,1,0));
	mat4 depthModelMatrix = mat4(1.0);
	mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

	// Send our transformation to the currently bound shader,
	// in the "MVP" uniform
	glUniformMatrix4fv(glGetUniformLocation(g_shader, "depthMVP"), 1, GL_FALSE, &depthMVP[0][0]);
	glEnable(GL_TEXTURE_2D);
	glUseProgram(g_shader);
	glUniform1i(glGetUniformLocation(g_shader, "texture0"), 0);
	glUniform1i(glGetUniformLocation(g_shader, "texture1"), 1);
	glUniform1f(glGetUniformLocation(g_shader, "shift"), shift);
	shift+=0.015f;
}

float noise(int x){
	x = (x<<13)^x;
	return ( 1.0 - ( (x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

int main(void)
{
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    window = glfwCreateWindow(640, 480, "", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);


	// Initilise GLEW
	// must be done after creating GL context (glutCreateWindow in this case)
	GLenum err = glewInit();
	if (GLEW_OK != err) { // Problem: glewInit failed, something is seriously wrong.
		cerr << "Error: " << glewGetErrorString(err) << endl;
		abort(); // Unrecoverable error
	}

	cout << "Using OpenGL " << glGetString(GL_VERSION) << endl;
	cout << "Using GLEW " << glewGetString(GLEW_VERSION) << endl;



    glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//
	initTexture(&g_texBannerfish, "../work/res/textures/Schooling_Bannerfish.jpg");
	initTexture(&g_texScissortail, "../work/res/textures/Scissor_Tail_Sergeant.jpg");
	initTexture(&g_texdolphin, "../work/res/textures/shortbeaked_dolphin.jpg");
	initTexture(&g_texHerring, "../work/res/textures/Atlantic_Herring.jpg");
	initTexture(&g_texSpongeBob, "../work/res/textures/spongebob.png");

	initShader();
	model = new Geometry("../work/res/models/schooling_bannerfish.obj", g_texBannerfish, g_shaderFish, false);
	modelScissortail = new Geometry("../work/res/models/scissortail_sergeant.obj", g_texScissortail, g_shaderFish, false);
	modelDolphin = new Geometry("../work/res/models/shortbeaked_dolphin.obj", g_texdolphin, g_shaderFish, true);
	modelHerring = new Geometry("../work/res/models/atlantic_herring.obj", g_texHerring, g_shaderFish, false);
	modelSpongeBob = new Geometry("../work/res/models/spongebob.obj", g_texSpongeBob, g_shaderFish, false);

	float size = 2000.f;
	sceneMng = new SceneManager(vec3(-size, -size/4, -size), vec3(size, size/4, size));

	PreyStrategy* bfs = new PreyStrategy();

	sceneMng->addSystem(200, model, bfs, 1.f, PREY);
	sceneMng->addSystem(200, modelScissortail, bfs, 0.8f, PREY);
	sceneMng->addSystem(200, modelHerring, bfs, 2.2f, PREY);
	sceneMng->addSystem(5, modelSpongeBob, bfs, 2.2f, PREY);

	PredatorStrategy *predStrat = new PredatorStrategy();
	sceneMng->addSystem(100, modelDolphin, predStrat, 10.f, PREDATOR);

	// Cullum
	sandTexture();
	//initCubeMap("work/res/textures/cubeMap.jpg");
	g_projection = new Projection();
	g_voroni = new Voronoi();
	g_voroni->render();

	// Callum

    vector<Coral> normCoral;
    vector<SpaceCoral> spaceCoral;

    //Make some coral
    int numOfCoral = 50;
    int maxX = 2000;
    int maxZ = 2000;
    for(int i=0; i<numOfCoral; i++){
    	int type = (int)(rand()%6);

    	float x = (int)((rand()%200/200.f)*maxX)- maxX/2;
    	float z = (int)((rand()%200/200.f)*maxZ)- maxZ/2;

		cout << x << " " << z;


		float y = g_projection->getHeight(x, z)+2;
		//if (type >= 3) type = 2;
		cout << y << " Here " << endl;
    	vec3 color = vec3((noise(i*3)+1)/2, (noise(i*11)+1)/2, (noise(i*7)+1)/2) ;
<<<<<<< HEAD
    	if(type<5){
    		Coral c = Coral("X", i, x, y, z, ((noise(i*7)+2)/2), color.r, color.g, color.b);
=======
    	if(type<3){
    		Coral c = Coral("X", i, x, y, z, 35*((noise(i*7)+2)/2), color.r, color.g, color.b);
>>>>>>> origin/master
    		int growthNum = (int)(((noise(i)+1)/2)*3)+2;;
    		if(type==0){
    			c.addRule('X', "D[#<^-DX]&[#>_+DX]&D[#<_DX]&[#>^DX]&");
    		}else if(type==1){
    			c.addRule('X', "D+D-[#<_DXD]&D[#>+DYD+^^]&[#^<+DX]&");
    			c.addRule('Y', "D-D-[#<_DYD]&D[#<_DXD<__]&[#_>-DY]&");
    		}else if(type==2){
    			c.addRule('X', "D[#-<DX]&[#+>DX]&[#_DX]&");
    		}
    		for(int g=0; g<growthNum; g++){
    			c.grow();
    		}
    		c.generateCoral(1);
    		normCoral.push_back(c);
			
    	}else{
    		float baseH = (((noise(i*3)+1)/2)*2);
    		float baseW = (((noise(i*3)+1)/2)+0.5f);
    		float shapeW = (((noise(i*3)+1)/2)*3.5f+0.5f);
    		float shapeH = (((noise(i*3)+1)/2)*4+2.0f);
    		vec3 pos = vec3(x, y, z);
    		Shape shape = Shape(pos + vec3(0, baseH+shapeH, 0), vec3(shapeW, shapeH, shapeW), false);
    		if(type==3){
    			//Rect
    			//shape = Shape(pos + vec3(0, baseH+5, 0), vec3(2, 5, 2), false);
    		}else if(type==4){
    			//Hollow sphere
    			shape = Shape(pos + vec3(0, baseH+shapeH/1.5f, 0), shapeH/1.5f+0.1f, true);
    		}else if(type==5){
    			//Sphere
    			shape = Shape(pos + vec3(0, baseH+shapeH/1.5f, 0), shapeH/1.5f+0.5f, false);
    		}
    		SpaceCoral s = SpaceCoral(pos, shape, baseH, baseW, color);
    		s.Grow();
    		spaceCoral.push_back(s);
    	}
		
    	cout<<"Coral "<<i<<endl;
		
    }

	// fps counter taken from http://www.opengl-tutorial.org/miscellaneous/an-fps-counter/
	double lastTime = glfwGetTime();
	int nbFrames = 0;

	while (!glfwWindowShouldClose(window))
	{
		// Measure speed
		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTime >= 1.0){ // If last prinf() was more than 1 sec ago
			// printf and reset timer
			printf("%f ms/frame, %f fps\n", 1000.0 / double(nbFrames), 1000.0 / (1000.0 / double(nbFrames)));
			nbFrames = 0;
			lastTime += 1.0;
		}

		// Cullum
		//
		//RENDER CALLS HERE
		//
		glActiveTexture(GL_TEXTURE1);
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glUseProgram(0);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glOrtho(-2,2,-2,2,-1,1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		g_voroni->render();

		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		/////////////
		ratio = width / (float)height;
		glViewport(0, 0, width, height);
		glClearColor(15 / 255.f, 70 / 255.f, 126 / 255.f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glEnable(GL_DEPTH_TEST);
		GLfloat zNear = 1.f;
		GLfloat zFar = 100000.0f;
		GLfloat aspect = float(width) / float(height);
		GLfloat fH = tan(radians(20.f)) * zNear;
		GLfloat fW = fH * aspect;
		glFrustum(-fW, fW, -fH, fH, zNear, zFar);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();


		mat4 lookat = lookAtLH(g_cameraPos, g_cameraPos + vec3(cos(radians(g_yRotation)), sin(radians(g_xRotation)), sin(radians(g_yRotation))), vec3(0,1,0));
		glMultMatrixf(&lookat[0][0]);


		glEnable(GL_TEXTURE_2D);
		glUseProgram(g_shaderSurface);
		glUniform1i(glGetUniformLocation(g_shader, "texture0"), 0);
		glUniform1i(glGetUniformLocation(g_shader, "texture1"), 1);
		glUniform1f(glGetUniformLocation(g_shader, "shift"), shift);
		// Water Surface
		glBegin(GL_QUADS);
				glColor3f(1, 1, 1);
				glNormal3f(0.0, 0.0, 1.0);
				glTexCoord2f(0.0, 0.0);
				glVertex3f(-10000.0, size/2, -10000.0);
				glTexCoord2f(0.0, 25.0);
				glVertex3f(-10000.0, size/2, 10000.0);
				glTexCoord2f(25.0, 25.0);
				glVertex3f(10000.0, size/2, 10000.0);
				glTexCoord2f(25.0, 0.0);
				glVertex3f(10000.0, size/2, -10000.0);
		glEnd();
		glFlush();

		// Cullum
		glEnable(GL_TEXTURE_2D);
		setupShader();	
		
		g_projection->renderLandscape();


		// Callum
		glEnable(GL_TEXTURE_2D);
		initLight();
		glUseProgram(g_shaderCoral);
		glUniform1i(glGetUniformLocation(g_shader, "texture0"), 0);
		glUniform1f(glGetUniformLocation(g_shader, "shift"), shift);
		glPushMatrix();
		
		for(Coral c : normCoral){
			glPushMatrix();
			c.draw();
			glPopMatrix();
		}
		for(SpaceCoral c : spaceCoral){
			glPushMatrix();
			c.Draw();
			glPopMatrix();
		}
		glPopMatrix();
		// Stephen
		// Enable Drawing texures
		glEnable(GL_TEXTURE_2D);
		if (g_simulate){
			sceneMng->stepSimulation();
		}
		glPushMatrix();
		  glTranslatef(0, 0, 0);
		  sceneMng->draw();
		glPopMatrix();


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
	delete bfs;
	delete model;
	delete modelScissortail;
	delete modelHerring;
	delete modelSpongeBob;
	delete sceneMng;
	delete predStrat;

    exit(EXIT_SUCCESS);
}
