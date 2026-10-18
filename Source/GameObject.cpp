//#include "GameObject.hpp"
//#include <cassert>
//
//// 实现 AddComponent 模板方法
//template<typename T, typename... Args>
//T* GameObject::AddComponent(Args&&... args) {
//    static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
//
//    // 1. 创建组件实例
//    auto pNewComponent = std::make_unique<T>(std::forward<Args>(args)...);
//    T* rawPtr = pNewComponent.get();
//
//    // 2. 检查是否已经存在相同类型的组件 (可选，取决于需求)
//     if (GetComponent<T>() != nullptr) {
//         // 处理重复添加的情况，例如警告或返回现有组件指针
//         assert(false && "Component already exists on GameObject!"); 
//         return GetComponent<T>();
//     }
//
//    // 3. 将组件添加到 vector 中
//    m_Components.push_back(std::move(pNewComponent));
//
//    // 4. 将组件的原始指针添加到 map 中，用于快速查找
//    m_ComponentMap[std::type_index(typeid(T))] = rawPtr;
//
//    return rawPtr; // 返回指向新组件的原始指针
//}
//
//// 实现 GetComponent 模板方法
//template<typename T>
//T* GameObject::GetComponent() const {
//    static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
//
//    // 1. 尝试从 map 中查找指定类型的组件
//    auto it = m_ComponentMap.find(std::type_index(typeid(T)));
//
//    // 2. 如果找到，返回对应的组件指针；否则返回 nullptr
//    if (it != m_ComponentMap.end()) {
//        return static_cast<T*>(it->second);
//    }
//    return nullptr;
//}
//
//// 为了使模板定义在链接时可用，通常需要显式实例化或放在 .inl 文件中
//// 或者，像上面一样，直接将实现放在 .cpp 文件中，但这要求所有使用该模板的编译单元都能看到完整定义
//// （这通常是通过将实现放在头文件或 .inl 文件中实现的，但对于基类来说，放在 .cpp 中比较麻烦）
//
//// 为了避免在 .cpp 中定义模板带来的链接问题，最常见的方式是将模板定义放在 .hpp 或 .inl 文件中。
//// 但既然您希望保持 .cpp 分离，我们需要在 .cpp 中显式实例化所有可能使用的类型，
//// 这在大型项目中是不可行的。因此，最实用的方法还是将定义放入 .hpp 或 .inl。
//
//// 所以，让我们将这两个函数的定义移到 .inl 文件中，或者直接移到 .hpp 文件的类定义之后。
//// 这里我们展示如何将它们放回 .hpp 文件中。