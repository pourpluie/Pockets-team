/*
 * Copyright (c) 2013 David Wicks, sansumbrella.com
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

#include "Locus.h"

using namespace pockets;
using namespace cinder;

MatrixAffine2f Locus2D::toMatrix() const
{
  MatrixAffine2f mat;
  mat.translate( position + registration_point );
  mat.rotate( rotation );
  mat.scale( scale );
  mat.translate( -registration_point );
  if( parent ){ mat = parent->toMatrix() * mat; }
  return mat;
}

ci::Vec2f Locus2D::getScale() const
{
  return parent ? parent->getScale() * scale : scale;
}

float Locus2D::getRotation() const
{
  return parent ? parent->getRotation() + rotation : rotation;
}

Vec2f Locus2D::getPosition() const
{
  return parent ? parent->toMatrix().transformPoint( position ) : position;
}

void Locus2D::detachFromParent()
{
  if( parent )
  {
    scale *= parent->getScale();
    rotation += parent->getRotation();
    position = parent->toMatrix().transformPoint( position );

    parent.reset();
  }
}
