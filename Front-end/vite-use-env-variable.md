#front-end/vite
## 2023.11.18

## Declare variables

The `import.meta.env` is a feature provided by modern bundlers like Vite or Snowpack, which gives you typed access to environment variables. This is very useful in the context of front-end development where we need to inject environment-specific values at build time.

In TypeScript, you can define an `env.d.ts` file to type check these variables. Here's how you can do it:

```ts
/// <reference types="vite/client" />

declare module '*.vue' {
  import type { DefineComponent } from 'vue'
  const component: DefineComponent<{}, {}, any>
  export default component
}

interface ImportMetaEnv {
  readonly VITE_API_BASE_URL: string
}
```

!!! Warning Note that with Vite, only variables prefixed with `VITE_` are exposed to your application code.

## Define and load variables

When using Vite, you can have multiple `.env` files in your project root. Vite adopts a mechanism very similar to the one used by `create-react-app`.

Here are the conventions that Vite uses:

1.  `.env`: Default.
2.  `.env.local`: Local overrides. This file is loaded for all environments except test.
3.  `.env.[mode]`: Environment-specific settings.
4.  `.env.[mode].local`: Local overrides of environment-specific settings.

Vite automatically loads these files, depending on the mode and whether local variables are present. The variables in later files will overwrite those in previous files, and the loaded environment variables will be available under `import.meta.env`.

For example, if you run `vite` (equivalent to running `vite --mode=development`), Vite will load the following files, if they are present:

-   `.env`
-   `.env.local`
-   `.env.development`
-   `.env.development.local`

And if you run `vite build` (equivalent to running `vite --mode=production`), it will load:

-   `.env`
-   `.env.local`
-   `.env.production`
-   `.env.production.local`

So, yes, you can define different environment variables in multiple `.env` files, and Vite will load them simultaneously based on the command/mode you're using.