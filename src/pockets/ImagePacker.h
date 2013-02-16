/*
 * Copyright (c) 2013 David Wicks
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the following
 * conditions are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once
#include "Pockets.h"
#include "cinder/Json.h"
#include "cinder/gl/Texture.h"

namespace pockets
{

/**
 Simple image packing
 Place all elements, largest-first, into a single Surface
 Height expands to fit
*/
class ImagePacker
{
  /*
   Source data for a sprite.
   Contains positional information and bitmap data.
   Drawable for previewing the texture.
   Converts to Json for export.

   Currently stores the original file since we might want to refer back to it when
   rebuilding sprite sheets. No support yet for reloading an existing sprite sheet
   with new versions of images.
   */
  class ImageData
  {
  public:
    ImageData( ci::Surface surface, const std::string &id, const ci::fs::path &file_path ):
    mSurface( surface )
    , mTexture( surface )
    , mId( id )
    , mFilePath( file_path )
    {}
    ci::Area            getBounds() const { return mSurface.getBounds(); }
    const ci::Surface&  getSurface() const { return mSurface; }
    ci::Vec2i           getLoc() const { return mLoc; }
    void                setLoc( const ci::Vec2i &loc ){ mLoc = loc; }
    void                draw()
    {
      ci::gl::draw( mTexture, mLoc );
    }
    ci::JsonTree        toJson()
    {
      using ci::JsonTree;
      JsonTree tree;
      tree.pushBack( JsonTree( "id", mId ) );
      tree.pushBack( JsonTree( "x1", mLoc.x ) );
      tree.pushBack( JsonTree( "y1", mLoc.y ) );
      tree.pushBack( JsonTree( "x2", mLoc.x + getBounds().getWidth() ) );
      tree.pushBack( JsonTree( "y2", mLoc.y + getBounds().getHeight() ) );
      tree.pushBack( JsonTree( "file", mFilePath.string() ) );
      return tree;
    }
  private:
    ci::Surface     mSurface;
    ci::gl::Texture mTexture;
    ci::Vec2i       mLoc = ci::Vec2i::zero();
    std::string     mId;
    ci::fs::path    mFilePath;
  };

public:
	ImagePacker();
	~ImagePacker();
  //! add an image to the sheet
  void          addImage( ci::Surface surface, const std::string &id, const ci::fs::path &file_path="" );
  void          addFont( const ci::Font &font, const std::string &glyphs );
  void          write( const ci::fs::path &destination_dir, const std::string &file_base_name );
  void          calculatePositions();
  void          draw();
  //! generates a surface containing all added images in their packed locations
  ci::Surface   packedSurface();
  //! returns a JSON-formatted description of all images and their packed locations
  ci::JsonTree  surfaceDescription();
private:
  //! width should be set to maximum desired width
  uint32_t                mWidth = 1024;
  //! height expands as elements are added
  uint32_t                mHeight = 1;
  std::vector<ImageData>  mImages;
};
} // ns pockets