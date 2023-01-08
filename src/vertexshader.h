#pragma once

#include "vertex.h"
#include "light.h"
#include "camera.h"
#include "transform.h"

namespace ramiel {

    class VS_PerTri {
        Camera& camera;
        Transform* transform;
    public:
        VS_PerTri(
            Camera& camera,
            Transform* transform
        ) :
            camera(camera),
            transform(transform)
        {}

        typedef Vertex_PerTri Vertex_Out;
        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            out.worldPos = (*transform)(in.pos);
            out.cameraPos = camera.getCameraCoord(out.worldPos);
            out.screenPos = camera.getScreenCoord(out.cameraPos);
            return out;
        }
    };


    class VS_PerTri_Textured {
        Camera& camera;
        Transform* transform;
    public:
        VS_PerTri_Textured(
            Camera& camera,
            Transform* transform
        ) :
            camera(camera),
            transform(transform)
        {}

        typedef Vertex_PerTri_Textured Vertex_Out;
        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            out.worldPos = (*transform)(in.pos);
            out.cameraPos = camera.getCameraCoord(out.worldPos);
            out.screenPos = camera.getScreenCoord(out.cameraPos);
            out.zinv = 1.0f / out.cameraPos[Z];
            out.texturePos = in.texture;
            return out;
        }
    };


    class VS_PerVertex {
        Camera& camera;
        LightingListSpecular lightingList;
        Vec3f surfaceColor;
        Transform* transform;
    public:
        VS_PerVertex(
            Camera& camera,
            Transform* transform,
            LightingListSpecular lightingList,
            Vec3f surfaceColor = vec3f_255
        ) :
            camera(camera),
            lightingList(lightingList),
            surfaceColor(surfaceColor / 255.0f),
            transform(transform)
        {}

        typedef Vertex_PerVertex Vertex_Out;
        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            Vec3f worldPos = (*transform)(in.pos);
            out.cameraPos = camera.getCameraCoord(worldPos);
            out.screenPos = camera.getScreenCoord(out.cameraPos);
            out.color = surfaceColor * lightingList.getAllLight(worldPos, in.normal);
            return out;
        }
    };


    class VS_PerVertex_Textured {
        Camera& camera;
        LightingListSpecular lightingList;
        Transform* transform;
    public:
        VS_PerVertex_Textured(
            Camera& camera,
            LightingListSpecular lightingList,
            Transform* transform
        ) :
            camera(camera),
            lightingList(lightingList),
            transform(transform)
        {}

        typedef Vertex_PerVertex_Textured Vertex_Out;
        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            Vec3f worldPos = (*transform)(in.pos);
            out.cameraPos = camera.getCameraCoord(worldPos);
            out.screenPos = camera.getScreenCoord(out.cameraPos);
            out.light = lightingList.getAllLight(worldPos, in.normal);
            out.zinv = 1.0f / out.cameraPos[Z];
            out.texturePos = in.texture;
            return out;
        }
    };


    class VS_PerPixel {
        Camera& camera;
        Transform* transform;
    public:
        VS_PerPixel(
            Camera& camera,
            Transform* transform
        ) :
            camera(camera),
            transform(transform)
        {}

        typedef Vertex_PerPixel Vertex_Out;
        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            out.worldPos = (*transform)(in.pos);
            out.cameraPos = camera.getCameraCoord(out.worldPos);
            out.screenPos = camera.getScreenCoord(out.cameraPos);
            out.zinv = 1.0f / out.cameraPos[Z];
            return out;
        }
    };


    class VS_PerPixel_Textured {
        Camera& camera;
        Transform* transform;
    public:
        VS_PerPixel_Textured(
            Camera& camera,
            Transform* transform
        ) :
            camera(camera),
            transform(transform)
        {}

        typedef Vertex_PerPixel_Textured Vertex_Out;
        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            out.worldPos = (*transform)(in.pos);
            out.cameraPos = camera.getCameraCoord(out.worldPos);
            out.screenPos = camera.getScreenCoord(out.cameraPos);
            out.zinv = 1.0f / out.cameraPos[Z];
            out.texturePos = in.texture;
            return out;
        }
    };


    class VS_PerPixel_Smooth {
        Camera& camera;
        Transform* transform;
    public:
        VS_PerPixel_Smooth(
            Camera& camera,
            Transform* transform
        ) :
            camera(camera),
            transform(transform)
        {}

        typedef Vertex_PerPixel_Smooth Vertex_Out;
        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            out.worldPos = (*transform)(in.pos);
            out.cameraPos = camera.getCameraCoord(out.worldPos);
            out.screenPos = camera.getScreenCoord(out.cameraPos);
            out.normal = in.normal;
            out.zinv = 1.0f / out.cameraPos[Z];
            return out;
        }
    };


    class VS_PerPixel_Smooth_Textured {
        Camera& camera;
        Transform* transform;
    public:
        VS_PerPixel_Smooth_Textured(
            Camera& camera,
            Transform* transform
        ) :
            camera(camera),
            transform(transform)
        {}

        typedef Vertex_PerPixel_Smooth_Textured Vertex_Out;
        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            out.worldPos = (*transform)(in.pos);
            out.cameraPos = camera.getCameraCoord(out.worldPos);
            out.screenPos = camera.getScreenCoord(out.cameraPos);
            out.normal = in.normal;
            out.zinv = 1.0f / out.cameraPos[Z];
            out.texturePos = in.texture;
            return out;
        }
    };

}
