#include "helper.h"

void initShaders() {

  idProgramShader = glCreateProgram();

  idVertexShader   = initVertexShader("shader.vert");
  idFragmentShader = initFragmentShader("shader.frag");

  glAttachShader(idProgramShader, idVertexShader);
  glAttachShader(idProgramShader, idFragmentShader);

  glLinkProgram(idProgramShader);

}

GLuint initVertexShader(const string& filename)
{
    string shaderSource;

    if (!readDataFromFile(filename, shaderSource)){
        cout << "Cannot find file name: " + filename << endl;
        exit(-1);
    }

    GLint length = shaderSource.length();
    const GLchar* shader = (const GLchar*) shaderSource.c_str();

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &shader, &length);
    glCompileShader(vs);

    char output[1024] = {0};
    glGetShaderInfoLog(vs, 1024, &length, output);
    printf("VS compile log: %s\n", output);

	  return vs;
}

GLuint initFragmentShader(const string& filename)
{
    string shaderSource;

    if (!readDataFromFile(filename, shaderSource)) {
        cout << "Cannot find file name: " + filename << endl;
        exit(-1);
    }

    GLint length = shaderSource.length();
    const GLchar* shader = (const GLchar*) shaderSource.c_str();

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &shader, &length);
    glCompileShader(fs);

    char output[1024] = {0};
    glGetShaderInfoLog(fs, 1024, &length, output);
    printf("FS compile log: %s\n", output);

	  return fs;
}

bool readDataFromFile(const string& fileName, string &data) {
    fstream myfile;

    myfile.open(fileName.c_str(), std::ios::in);

    if (myfile.is_open()) {
        string curLine;

        while (getline(myfile, curLine)){
            data += curLine;
            if (!myfile.eof())
                data += "\n";
        }

        myfile.close();
    }
    else
        return false;

    return true;
}

/*void initTexture(char * filename , char* filename2, int *w, int *h)
{
    int width, height;

    unsigned char *raw_image = NULL;
    unsigned char *raw_image2 = NULL;
    int bytes_per_pixel = 3;   // or 1 for GRACYSCALE images 
    int color_space = JCS_RGB; // or JCS_GRAYSCALE for grayscale images 

    // these are standard libjpeg structures for reading(decompression) 
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    // libjpeg data structure for storing one row, that is, scanline of an image 
    JSAMPROW row_pointer[1];

    FILE *infile = fopen( filename, "rb" );
    unsigned long location = 0;
    int i = 0, j = 0;

    if ( !infile )
    {
        printf("Error opening jpeg file %s\n!", filename );
        return;
    }
    printf("Texture filename = %s\n",filename);

    // here we set up the standard libjpeg error handler 
    cinfo.err = jpeg_std_error( &jerr );
    // setup decompression process and source, then read JPEG header 
    jpeg_create_decompress( &cinfo );
    // this makes the library read from infile 
    jpeg_stdio_src( &cinfo, infile );
    // reading the image header which contains image information 
    jpeg_read_header( &cinfo, TRUE );
    // Start decompression jpeg here 
    jpeg_start_decompress( &cinfo );

    // allocate memory to hold the uncompressed image 
    raw_image = (unsigned char*)malloc( cinfo.output_width*cinfo.output_height*cinfo.num_components );
    // now actually read the jpeg into the raw buffer 
    row_pointer[0] = (unsigned char *)malloc( cinfo.output_width*cinfo.num_components );
    // read one scan line at a time 
    while( cinfo.output_scanline < cinfo.image_height )
    {
        jpeg_read_scanlines( &cinfo, row_pointer, 1 );
        for( i=0; i<cinfo.image_width*cinfo.num_components;i++)
            raw_image[location++] = row_pointer[0][i];
    }

    height = cinfo.image_height;
    width = cinfo.image_width;

    // filename 2
    FILE *infile2 = fopen( filename2, "rb" );
    //unsigned long 
    location = 0;
    //int 
    i = 0;
     j = 0;

    if ( !infile2 )
    {
        printf("Error opening jpeg file %s\n!", filename2 );
        return;
    }
    printf("Texture filename = %s\n",filename2);

    // here we set up the standard libjpeg error handler 
    cinfo.err = jpeg_std_error( &jerr );
    // setup decompression process and source, then read JPEG header 
    jpeg_create_decompress( &cinfo );
    // this makes the library read from infile 
    jpeg_stdio_src( &cinfo, infile2 );
    // reading the image header which contains image information 
    jpeg_read_header( &cinfo, TRUE );
    // Start decompression jpeg here 
    jpeg_start_decompress( &cinfo );

    // allocate memory to hold the uncompressed image 
    raw_image2 = (unsigned char*)malloc( cinfo.output_width*cinfo.output_height*cinfo.num_components );
    // now actually read the jpeg into the raw buffer 
    row_pointer[0] = (unsigned char *)malloc( cinfo.output_width*cinfo.num_components );
    // read one scan line at a time 
    while( cinfo.output_scanline < cinfo.image_height )
    {
        jpeg_read_scanlines( &cinfo, row_pointer, 1 );
        for( i=0; i<cinfo.image_width*cinfo.num_components;i++)
            raw_image2[location++] = row_pointer[0][i];
    }

    height = cinfo.image_height;
    width = cinfo.image_width;

    glGenTextures(1,idJpegTexture);
    glBindTexture(GL_TEXTURE_2D, idJpegTexture[0]);
    glActiveTexture(GL_TEXTURE1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, raw_image);

    glBindTexture(GL_TEXTURE_2D, idJpegTexture[1]);
    glActiveTexture(GL_TEXTURE0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, raw_image2);


    *w = width;
    *h = height;

    //glGenerateMipmap(GL_TEXTURE_2D);
    // wrap up decompression, destroy objects, free pointers and close open files 
    jpeg_finish_decompress( &cinfo );
    jpeg_destroy_decompress( &cinfo );
    free( row_pointer[0] );
    free( raw_image );
    fclose( infile );

}*/

void initTexture(char * filename,char * filename2 , int *w, int *h)
{
    int width, height;

    unsigned char *raw_image = NULL;
    int bytes_per_pixel = 3;   // or 1 for GRACYSCALE images 
    int color_space = JCS_RGB; // or JCS_GRAYSCALE for grayscale images 

    // these are standard libjpeg structures for reading(decompression) 
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    // libjpeg data structure for storing one row, that is, scanline of an image 
    JSAMPROW row_pointer[1];

    FILE *infile = fopen( filename2, "rb" );
    unsigned long location = 0;
    int i = 0, j = 0;

    if ( !infile )
    {
        printf("Error opening jpeg file %s\n!", filename2 );
        return;
    }
    printf("Texture filename2 = %s\n",filename2);

    // here we set up the standard libjpeg error handler 
    cinfo.err = jpeg_std_error( &jerr );
    // setup decompression process and source, then read JPEG header 
    jpeg_create_decompress( &cinfo );
    // this makes the library read from infile 
    jpeg_stdio_src( &cinfo, infile );
    // reading the image header which contains image information 
    jpeg_read_header( &cinfo, TRUE );
    // Start decompression jpeg here 
    jpeg_start_decompress( &cinfo );

    // allocate memory to hold the uncompressed image 
    raw_image = (unsigned char*)malloc( cinfo.output_width*cinfo.output_height*cinfo.num_components );
    // now actually read the jpeg into the raw buffer 
    row_pointer[0] = (unsigned char *)malloc( cinfo.output_width*cinfo.num_components );
    // read one scan line at a time 
    while( cinfo.output_scanline < cinfo.image_height )
    {
        jpeg_read_scanlines( &cinfo, row_pointer, 1 );
        for( i=0; i<cinfo.image_width*cinfo.num_components;i++)
            raw_image[location++] = row_pointer[0][i];
    }

    height = cinfo.image_height;
    width = cinfo.image_width;

//HEIGHT____HEIGHT____HEIGHT____HEIGHT____HEIGHT____HEIGHT____HEIGHT____HEIGHT____
    unsigned char *raw_image2 = NULL;
    bytes_per_pixel = 3;   // or 1 for GRACYSCALE images 
    color_space = JCS_RGB; // or JCS_GRAYSCALE for grayscale images 

    // these are standard libjpeg structures for reading(decompression) 
    struct jpeg_decompress_struct cinfo2;
    struct jpeg_error_mgr jerr2;

    // libjpeg data structure for storing one row, that is, scanline of an image 
    JSAMPROW row_pointer2[1];

    FILE *infile2 = fopen( filename, "rb" );
    unsigned long location2 = 0;
    i = 0;

    if ( !infile2 )
    {
        printf("Error opening jpeg file %s\n!", filename );
        return;
    }
    printf("Texture filename = %s\n",filename);

    // here we set up the standard libjpeg error handler 
    cinfo2.err = jpeg_std_error( &jerr2 );
    // setup decompression process and source, then read JPEG header 
    jpeg_create_decompress( &cinfo2 );
    // this makes the library read from infile2 
    jpeg_stdio_src( &cinfo2, infile2 );
    // reading the image header which contains image information 
    jpeg_read_header( &cinfo2, TRUE );
    // Start decompression jpeg here 
    jpeg_start_decompress( &cinfo2 );

    // allocate memory to hold the uncompressed image 
    raw_image2 = (unsigned char*)malloc( cinfo2.output_width*cinfo2.output_height*cinfo2.num_components );
    // now actually read the jpeg into the raw buffer 
    row_pointer2[0] = (unsigned char *)malloc( cinfo2.output_width*cinfo2.num_components );
    // read one scan line at a time 
    while( cinfo2.output_scanline < cinfo2.image_height )
    {
        jpeg_read_scanlines( &cinfo2, row_pointer2, 1 );
        for( i=0; i<cinfo2.image_width*cinfo2.num_components;i++)
            raw_image2[location2++] = row_pointer2[0][i];
    }
    glGenTextures(2, idJpegTexture);
    glBindTexture(GL_TEXTURE_2D, idJpegTexture[1]);
    glActiveTexture(GL_TEXTURE1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, raw_image2);

    //glGenerateMipmap(GL_TEXTURE_2D);
    // wrap up decompression, destroy objects, free pointers and close open files 
    jpeg_finish_decompress( &cinfo2 );
    jpeg_destroy_decompress( &cinfo2 );
    free( row_pointer2[0] );
    free( raw_image2 );
    fclose( infile2 );
    //HEIGHT____HEIGHT____HEIGHT____HEIGHT____HEIGHT____HEIGHT____HEIGHT____HEIGHT____

    //glGenTextures(2, idJpegTexture);
    glBindTexture(GL_TEXTURE_2D, idJpegTexture[0]);
    glActiveTexture(GL_TEXTURE0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, raw_image);

    *w = width;
    *h = height;

    glGenerateMipmap(GL_TEXTURE_2D);
    // wrap up decompression, destroy objects, free pointers and close open files 
    jpeg_finish_decompress( &cinfo );
    jpeg_destroy_decompress( &cinfo );
    free( row_pointer[0] );
    free( raw_image );
    fclose( infile );

    

}
