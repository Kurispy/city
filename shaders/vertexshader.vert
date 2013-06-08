#version 110

varying vec3 ex_Normal;		//this is passed to fragment shader for "lighting" (therefore the "varying" keyword)

 
void main(void)
{
	//the vertex shader can access active Projection and Modelview transformations
	//these transformations need to be applied in order to perform the operations specified by "glRotate,glTranslate,glScale" and camera projections

	gl_TexCoord[0]=gl_MultiTexCoord0; //Prepare gl_TexCoord[0] for fragment shader

	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;  	//rotate, translate, and project positions

    ex_Normal = vec3(gl_ModelViewMatrix*vec4(gl_Normal,0.0));	//rotate normals - note 0.0 w component for directions (as you can see in the fragment shader, we are here not interested in projecting normals, like we do for positions)
	//ex_Normal is a "varying" variable and will be available in the fragment shader

}
