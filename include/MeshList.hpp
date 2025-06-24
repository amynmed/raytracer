#pragma once

#include "Mesh.hpp"

#include <vector>
#include <memory>
#include "Interval.hpp"


class MeshList: public Mesh 
{

        public:

        std::vector<std::shared_ptr<Mesh>> meshes;

        MeshList() {}

        MeshList(std::shared_ptr<Mesh> object) 
        { 
                add(object); 
        }

        void clear() 
        { 
                meshes.clear(); 
        }

        void add(std::shared_ptr<Mesh> object) 
        {
                meshes.push_back(object);
        }

        bool hit(const Ray& r, Interval interval, Intersection& rec) const override 
        {
                Intersection temp_rec;

                bool hit_anything   = false;
                auto closest_so_far = interval.max;

                for (const auto& object : meshes) 
                {
                        if (object->hit(r, Interval(interval.min, closest_so_far), temp_rec)) 
                        {
                                hit_anything   = true;
                                closest_so_far = temp_rec.t;
                                rec            = temp_rec;
                        }
                }

                return hit_anything;
        }

};