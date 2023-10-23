//
// Created by MINH NHUT on 6/8/2023.
//

#ifndef NATUREOFCODE_RESOURCEHOLDER_HPP
#define NATUREOFCODE_RESOURCEHOLDER_HPP

#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <cassert>

template <typename Resource, typename Identifier>
class ResourceHolder {
   public:
	void load(Identifier id, const std::string& filename);

	Resource& get(Identifier id);
	const Resource& get(Identifier id) const;

   private:
	void insertResource(Identifier id, std::unique_ptr<Resource> resource);

   private:
	std::map<Identifier, std::unique_ptr<Resource>> mResourceMap;
};

#include "ResourceHolder.inl"

#endif  //NATUREOFCODE_RESOURCEHOLDER_HPP
