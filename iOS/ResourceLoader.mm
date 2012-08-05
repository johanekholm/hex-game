/*
 *  ResourceLoader.h
 *  hex-game
 *
 *  Created by Micke Prag on 2011-08-15.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ResourceLoader.h"
#include "Texture2D.h"

ResourceLoader::ResourceLoader()
:AbstractResourceLoader()
{
}

ResourceLoader::~ResourceLoader() {
}

GLuint ResourceLoader::loadTexture(const std::string &filename) {
	Texture2D *tex = [[Texture2D alloc] initWithImage: [UIImage imageNamed: [NSString stringWithCString:filename.c_str() encoding:[NSString defaultCStringEncoding] ]]];
	GLuint texRef = [tex name];
	[tex dealloc];
	return texRef;
}

bool ResourceLoader::doesFileExist(const std::string& filename, int dir) {
	NSString * path;
	
	path = [ResourceLoader::getPathToDir(dir) stringByAppendingPathComponent: [NSString stringWithCString:filename.c_str() encoding:[NSString defaultCStringEncoding]]];
		
	return [[NSFileManager defaultManager] fileExistsAtPath:path];
}

NSString* ResourceLoader::getPathToDir(int dir) {
	NSArray *dirPaths;

	switch (dir) {
		case DirNS::RESOURCE:
			return [[NSBundle mainBundle] resourcePath];

		case DirNS::SAVE:
			dirPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
			return [dirPaths objectAtIndex:0];

		default:
			return @"";
	}
}

std::string ResourceLoader::loadFileAsString(const std::string &filename) {
	NSString * path;

	path = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent: [NSString stringWithCString:filename.c_str() encoding:[NSString defaultCStringEncoding]]];

	if ([[NSFileManager defaultManager] fileExistsAtPath:path]) {
		NSString *pythonString = [NSString stringWithContentsOfFile:path encoding:[NSString defaultCStringEncoding] error:NULL ];
		if (pythonString) {
			return [pythonString UTF8String];
		}
	}
	return "";
}

std::string ResourceLoader::loadFileAsString(const std::string &filename, int dir) {
	NSString * path;
	NSFileManager* fileManager;
	
	path = [ResourceLoader::getPathToDir(dir) stringByAppendingPathComponent: [NSString stringWithCString:filename.c_str() encoding:[NSString defaultCStringEncoding]]];

	DEBUGLOG("Load-file path: %s", [path UTF8String]);

	fileManager = [NSFileManager defaultManager];
		 
	if ([fileManager fileExistsAtPath:path]) {
		NSString *fileContent = [NSString stringWithContentsOfFile:path encoding:[NSString defaultCStringEncoding] error:NULL ];
		if (fileContent) {
			return [fileContent UTF8String];
		}
	}
	return "";
}

void ResourceLoader::writeStringToFile(const std::string& textString, const std::string& filename, int dir) {
	NSString * path;
	
	path = [ResourceLoader::getPathToDir(dir) stringByAppendingPathComponent: [NSString stringWithCString:filename.c_str() encoding:[NSString defaultCStringEncoding]]];
	
	DEBUGLOG("Write-file path: %s", [path UTF8String]);
		
	[[NSString stringWithCString:textString.c_str() encoding:[NSString defaultCStringEncoding]] writeToFile:path atomically:TRUE encoding:NSUTF8StringEncoding error:NULL];

}

