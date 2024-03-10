#front-end/vite #front-end/vue
## 2023.11.18

> https://vue-amap.guyixi.cn/zh-cn/introduction/init.html

## For auto import

**1. Install package**

```bash
$ npm install -D unplugin-vue-components unplugin-auto-import @vuemap/unplugin-resolver
```

**2. Add to `main.ts`**

```ts
import App from './App.vue'
import {initAMapApiLoader} from '@vuemap/vue-amap';
import '@vuemap/vue-amap/dist/style.css'
initAMapApiLoader({
  key: ''
  securityJsCode: '' // 新版key需要配合安全密钥使用
  //Loca:{
  //  version: '2.0.0'
  //} // 如果需要使用loca组件库，需要加载Loca
})

createApp(App).mount('#app')
```

**3. Add to `vite.config.ts`**

```ts
import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'
import AutoImport from 'unplugin-auto-import/vite'
import Components from 'unplugin-vue-components/vite'
import {VueAmapResolver} from '@vuemap/unplugin-resolver'

// https://vitejs.dev/config/
export default defineConfig({
  plugins: [
    vue(),
    AutoImport({
      resolvers: [VueAmapResolver()],
    }),
    Components({
      resolvers: [VueAmapResolver()],
    }),
  ]
})
```

!!! Warning Because of element-plus will auto import `el` prefix components, while `el-map` is not provided by element plus

Exclude it from element plus auto import

```ts
import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'
import AutoImport from 'unplugin-auto-import/vite'
import Components from 'unplugin-vue-components/vite'
import { ElementPlusResolver } from 'unplugin-vue-components/resolvers'
import {VueAmapResolver} from '@vuemap/unplugin-resolver'

// https://vitejs.dev/config/
export default defineConfig({
  plugins: [
    vue(),
    AutoImport({
      resolvers: [ElementPlusResolver({
        exclude: /^ElAmap[A-Z]*/
      }),VueAmapResolver()],
    }),
    Components({
      resolvers: [ElementPlusResolver({
        exclude: /^ElAmap[A-Z]*/
      }),VueAmapResolver()],
    }),
  ]
})
```

## Usage

```html
<template>
  <div class="map-page-container">
    <el-amap
      :center="center"
      :zoom="zoom"
      @init="init"
    />
  </div>
  <div class="toolbar">
    <button @click="add()">
      添加标号
    </button>
  </div>
</template>

<script lang="ts" setup>
import {ref} from "vue";
import {ElAmap} from "@vuemap/vue-amap";

const zoom = ref(12);
const center = ref([121.59996, 31.197646]);
let map = null;
declare let AMap: any // global variable
const init = (e) => {
  const marker = new AMap.Marker({
    position: [121.59996, 31.197646]
  });
  e.add(marker);
  map = e;
  console.log('map init: ', map)
}
const add = () => {
  const marker = new AMap.Marker({
    position: [121.59996, 31.177646]
  });
  map.add(marker);
}
</script>

<style>
</style>
```