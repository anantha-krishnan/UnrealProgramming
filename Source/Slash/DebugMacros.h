#pragma once
#include "DrawDebugHelpers.h"

#define DRAW_SPHERE(loc,persist)            if (GetWorld()) DrawDebugSphere(GetWorld(), loc, 30.0, 24, FColor::Green, persist, 10.f);
#define DRAW_LINE(locB,locE,persist)        if (GetWorld()) DrawDebugLine(GetWorld(), locB, locE, FColor::Red, persist,10.f);
#define DRAW_POINT(loc,persist)             if (GetWorld()) DrawDebugPoint(GetWorld(), loc, 10.0, FColor::Blue, persist,10.f);
#define DRAW_CAPSULE(loc,actQat,persist)    if (GetWorld()) DrawDebugCapsule(GetWorld(), loc, 50.0, 10.0, actQat, FColor::MakeRandomColor(), persist,10.f);
#define DRAW_VECTOR(locB,locE,persist)      if (GetWorld()) \
    { \
        DRAW_LINE(locB,locE,persist);    \
        DRAW_POINT(locE,persist);    \
    }
