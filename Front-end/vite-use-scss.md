#front-end/vue #front-end/vite
## 2023.11.17
> https://juejin.cn/post/7065929016569495565

## Import scss module

**1. install sass package**

```bash
$ npm install sass --save-dev
```

**2. Setup scss preprocessor**

Add following lines in `vite.config.ts` 

```js
export default defineConfig({
  ...
  css: {
    preprocessorOptions: {
      scss: {
        additionalData: `@import "@/assets/main.scss";`
      }
    }
  }
  ...
})
```

!!! Warning If not working
    
    There must be a `<style lang="scss">` in one `.vue` file or one `import scss` in `main.ts` or else the preprocessor won't work!
