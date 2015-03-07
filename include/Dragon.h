// Copyright (c) 2009-2015, Pierre-Andre Saulais <pasaulais@free.fr>
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer. 
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef INITIALS_DRAGON_H
#define INITIALS_DRAGON_H

#include <inttypes.h>
#include "Material.h"
#include "RenderState.h"

class Scene;

class Dragon : public StateObject
{
public:
    enum Kind
    {
        Floating,
        Flying,
        Jumping
    };

    Dragon(Kind kind, RenderState *state);

    void setDetailLevel(int level);

    Material & tongueMaterial();
    Material & scalesMaterial();
    Material & wingMaterial();
    Material & membraneMaterial();

    float frontLegsAngle() const;
    float alpha() const;
    float beta() const;
    void setAlpha(float v);
    void setBeta(float v);

    void draw();

    void drawUpper();
    void drawHead();
    void drawTongue();
    void drawJoint();

    void drawBody();
    void drawChest();

    void drawWing();
    void drawWingPart();
    void drawWingOuter();
    void drawWingMembrane();

    void drawPaws();
    void drawPaw();

    void drawTail();
    void drawTailEnd();

    void animate(float t);

private:
    Kind m_kind;
    uint32_t m_jointParts;
    uint32_t m_chestParts;
    uint32_t m_tailEndParts;
    Material m_tongueMaterial;
    Material m_scalesMaterial;
    Material m_wingMaterial;
    Material m_membraneMaterial;
    float theta_jaw;
    float theta_head_z;
    float theta_head_y;
    float theta_neck;
    float theta_wing;
    float theta_wing_joint;
    float theta_front_legs;
    float theta_back_legs;
    float theta_paw;
    float theta_tail;
    float m_alpha;              //
    float m_beta;               // animation-dependent variables
};

#endif
