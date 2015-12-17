#include "NCCDemo.h"
#include "homography.h"
using namespace std;

int calculatePixelValue(Color value)
{
	return (value.r+ value.b + value.g)/3;
}

void detectGlare(vector<Color> * colors, vector<Color> * colors2, vector<bool> * glarePixels)
{
	int pixel1, pixel2;
	for (unsigned int pixel_count = 0; pixel_count < colors->size(); pixel_count++)
	{
		pixel1 = calculatePixelValue((*colors)[pixel_count]);
		pixel2 = calculatePixelValue((*colors2)[pixel_count]); 
		if ((pixel1 > 250 || pixel2 > 250) && abs(pixel1 - pixel2) > 50)
		{
			glarePixels->push_back(true);
		}
		else
		{
			glarePixels->push_back(false);
		}
	}
}

double calculateNCCWithoutGlare(vector<PixelLoc> *interior, double * best, Image *myimg, Image *myimgOther, Image *glareImage)
{
	vector<bool> glarePixels; 
	Color black(0,0,0);
 	Color white(255,255,255);
	vector<Color> signal1,signal2;
	double point[2];

	for(unsigned int i=0; i<interior->size(); ++i)
	{
		homography(interior[0][i].x + 0.5 , interior[0][i].y + 0.5, best, point);
        Coord mycoord(point[0], point[1]);
        if(inImage(myimg,mycoord)){
            signal1.push_back(asInterpolatedColor(mycoord, myimg));
        } else {
            signal1.push_back(black);
            signal2.push_back(white);
            continue;
        }
        signal2.push_back(myimgOther->getPixel(interior[0][i]));
    }
	
	detectGlare(&signal1, &signal2, &glarePixels);

	vector<Color> image1;
	vector<Color> image2;
	Color blue(0,0,100);

	for (unsigned int pixel_count = 0; pixel_count < signal1.size(); pixel_count++)
	{
		if (glarePixels[pixel_count] != true)
		{
			image1.push_back(signal1[pixel_count]);
			image2.push_back(signal2[pixel_count]);
		}
		else
		{
   			homography((*interior)[pixel_count].x, (*interior)[pixel_count].y, best, point);
  		    PixelLoc loc((int)point[0], (int)point[1]);
      		glareImage->setPixel(loc,blue);
		}
	}

	cout << image1 << endl;
	cout << image2 << endl;
	return calculate_normalized_correlation(image1, image2);
}