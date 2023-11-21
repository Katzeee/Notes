#front-end/vite #front-end/vue
## 2023.11.17

> https://vuetifyjs.com/en/getting-started/installation/#existing-projects  
> https://vuetifyjs.com/en/features/icon-fonts/#mdi-css 

## Setup


**1. Install vuetify**

```bash
$ npm i vite-plugin-vuetify
```

**2. Install icons**

```bash
$ npm i @mdi/font -D
```

**3. Use in main.ts**

```ts
import 'vuetify/styles'
import { createVuetify } from 'vuetify'
import { aliases, mdi } from 'vuetify/iconsets/mdi'
const vuetify = createVuetify({
  icons: {
    defaultSet: 'mdi',
    aliases,
    sets: {
      mdi
    }
  }
})
app.use(vuetify)
```

**4. auto-import components**

```ts
import vuetify from 'vite-plugin-vuetify'

export default defineConfig({
  plugins: [
    ...
    vuetify({ autoImport: true }) // Enabled by default
    ...
  ],
  ...
})
```

## `v-overlay`

If you specified `activator`, that `activator` will be automatically implented `@click`, so you are not supposed to set it again.

If you want to use `location` and `origin` attributes, you should set `location-strategy` to `connected`.

```html
<v-overlay
    scroll-strategy="close"
    location="bottom start"
    origin="top start"
    activator="parent"
    transition="scale-transition"
    location-strategy="connected"
    scrim="transparent"
    :max-width="rightWidth"
  >
      <slot name="overlay"></slot>
</v-overlay>
```
