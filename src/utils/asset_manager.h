//
// Created by ktsq on 2021/8/24.
//

#ifndef CSUGL_ASSET_MANAGER_H
#define CSUGL_ASSET_MANAGER_H

#include "../core/core.h"
#include <unordered_map>

namespace csugl::am {

    enum class AssetPtrType {
        ORG_PTR,
        REF_PTR
    };

    template<typename Key, typename Val, AssetPtrType type>
    class asset_manager {
    };

    template<typename Key, typename Val>
    class asset_manager<Key, Val, AssetPtrType::ORG_PTR> {
    public:
        void add(const Key &key, Val *val) {
            pool.try_emplace(key, val);
        }

        Val *get(const Key &key) {
            const auto &res = pool.find(key);
            return res == pool.end() ? nullptr : res->second;
        }

    private:
        std::unordered_map<Key, Val *> pool;
    };

    template<typename Key, typename Val>
    class asset_manager<Key, Val, AssetPtrType::REF_PTR> {
    public:
        void add(const Key &key, const Ref<Val> &val) {
            pool.try_emplace(key, val);
        }

        Ref<Val> get(const Key &key) {
            const auto &res = pool.find(key);
            return res == pool.end() ? nullptr : res->second;
        }

    private:
        std::unordered_map<Key, Ref<Val>> pool;
    };

    template<typename Key, typename Val>
    using Org_AM = asset_manager<Key, Val, AssetPtrType::ORG_PTR>;

    template<typename Key, typename Val>
    using Ref_AM = asset_manager<Key, Val, AssetPtrType::REF_PTR>;

    template<typename Val>
    using Org_Str_AM = asset_manager<std::string, Val, AssetPtrType::ORG_PTR>;

    template<typename Val>
    using Ref_Str_AM = asset_manager<std::string, Val, AssetPtrType::REF_PTR>;

}

#ifndef SGT_Org_AM
#define SGT_Org_AM(KEY_TYPE, VAL_TYPE) csugl::singleton<csugl::am::Org_AM<KEY_TYPE, VAL_TYPE>>::getInstance()
#endif

#ifndef SGT_Ref_AM
#define SGT_Ref_AM(KEY_TYPE, VAL_TYPE) csugl::singleton<csugl::am::Ref_AM<KEY_TYPE, VAL_TYPE>>::getInstance()
#endif

#ifndef SGT_Org_Str_AM
#define SGT_Org_Str_AM(VAL_TYPE) csugl::singleton<csugl::am::Org_Str_AM<VAL_TYPE>>::getInstance()
#endif

#ifndef SGT_Ref_Str_AM
#define SGT_Ref_Str_AM(VAL_TYPE) csugl::singleton<csugl::am::Ref_Str_AM<VAL_TYPE>>::getInstance()
#endif

#endif //CSUGL_ASSET_MANAGER_H
