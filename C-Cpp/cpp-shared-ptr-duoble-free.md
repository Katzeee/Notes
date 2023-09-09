#c-cpp
## 2023.09.09

## Background

I got a `corrupted size vs. prev_size while consolidating` when the program exit.


## Debug

After debugging I found it happened in the destructor when it try to destruct a string.

Also, I found this bug always caused by incorrect memory access.

Then I realize this bug occurs after I choose to use shared pointer rather the raw pointer to manage the Shader object because it will be used by many Meshes.

The bug I wrote:

```cpp
Model model{};
auto shader = std::make_shared<Shader>();
model.SetShader(shader.get()); // incorrect

void Model::SetShader(Shader *const shader) {
    for (auto &&mesh : meshes_) {
        mesh.SetSHader(shader);
    }
}

void Mesh::SetShader(SHader *const shader) {
    shader_ = std::make_shared<Shader>(shader);
}
```

This code creates several `shared_ptr` whose use count is 1, which leads to several free of one same object.


