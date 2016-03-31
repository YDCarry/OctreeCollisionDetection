//
//  LoadImage.h
//  OctreeCollisionDetection
//
//  Created by Ydy on 16/3/30.
//  Copyright © 2016年 Ydy. All rights reserved.
//

#ifndef LoadImage_h
#define LoadImage_h

class Image
{
    public:
    Image(char *_pixels, int _width, int _height);
    ~Image();
    
    char *pixels;
    int width;
    int height;
};

Image* LoadBMP(const char* file_name);

#endif /* LoadImage_h */
