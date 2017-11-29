// ======================================================================== //
// Copyright 2009-2017 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#pragma once

#include "default.h"
#include "ray.h"

namespace embree
{
  /* Hit structure for K hits */
  template<int K>
  struct HitK
  {
    /* Default construction does nothing */
    __forceinline HitK() {}

    /* Constructs a hit */
    __forceinline HitK(const vint<K>& instID, const vint<K>& geomID, const vint<K>& primID, const vfloat<K>& u, const vfloat<K>& v, const vfloat<K>& t, const Vec3vf<K>& Ng)
      : Ng(Ng), t(t), u(u), v(v), primID(primID), geomID(geomID), instID(instID) {}

    /* Returns the size of the hit */
    static __forceinline size_t size() { return K; }

  public:
    Vec3vf<K> Ng;  // geometry normal
    vfloat<K> t;         // hit distance
    vfloat<K> u;         // barycentric u coordinate of hit
    vfloat<K> v;         // barycentric v coordinate of hit
    vint<K> primID;      // primitive ID
    vint<K> geomID;      // geometry ID
    vint<K> instID;      // instance ID
  };

  /* Specialization for a single hit */
  template<>
  struct HitK<1>
  {
     /* Default construction does nothing */
    __forceinline HitK() {}

    /* Constructs a hit */
    __forceinline HitK(int instID, int geomID, int primID, float u, float v, float t, const Vec3fa& Ng)
      : Ng(Ng.x,Ng.y,Ng.z), t(t), u(u), v(v), primID(primID), geomID(geomID), instID(instID) {}

    /* Returns the size of the hit */
    static __forceinline size_t size() { return 1; }

  public:
    Vec3<float> Ng;  // geometry normal
    float t;         // hit distance
    float u;         // barycentric u coordinate of hit
    float v;         // barycentric v coordinate of hit
    int primID;      // primitive ID
    int geomID;      // geometry ID
    int instID;      // instance ID
  };

  /* Shortcuts */
  typedef HitK<1>  Hit;
  typedef HitK<4>  Hit4;
  typedef HitK<8>  Hit8;
  typedef HitK<16> Hit16;

  /* Outputs hit to stream */
  template<int K>
  inline std::ostream& operator<<(std::ostream& cout, const HitK<K>& ray)
  {
    return cout << "{ " << std::endl
                << "  primID = " << ray.primID <<  std::endl
                << "  geomID = " << ray.geomID << std::endl
                << "  instID = " << ray.instID << std::endl
                << "  u = " << ray.u <<  std::endl
                << "  v = " << ray.v << std::endl
                << "  t = " << ray.v << std::endl
                << "  Ng = " << ray.Ng
                << "}";
  }

  __forceinline void copyHitToRay(Ray &ray, const Hit& hit)
  {
    ray.Ng   = hit.Ng;
    ray.primID = hit.primID;
    ray.geomID = hit.geomID;
    ray.instID = hit.instID;
    ray.u    = hit.u;
    ray.v    = hit.v;
    ray.tfar() = hit.t;
  }

  template<int K>
    __forceinline void copyHitToRay(const vbool<K> &mask, RayK<K> &ray, const HitK<K> &hit)
  {
    vfloat<K>::storeu(mask,&ray.Ng.x, hit.Ng.x);
    vfloat<K>::storeu(mask,&ray.Ng.y, hit.Ng.y);
    vfloat<K>::storeu(mask,&ray.Ng.z, hit.Ng.z);
    vint<K>::storeu(mask,&ray.primID, hit.primID);
    vint<K>::storeu(mask,&ray.geomID, hit.geomID);
    vint<K>::storeu(mask,&ray.instID, hit.instID);
    vfloat<K>::storeu(mask,&ray.u, hit.u);
    vfloat<K>::storeu(mask,&ray.v, hit.v);
    vfloat<K>::storeu(mask,&ray._tfar, hit.t);
  }
}
