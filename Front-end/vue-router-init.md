#front-end/vue
## 2023.11.17

> https://www.jb51.net/javascript/295992mho.htm

## Route push only change url but not the content

For using `pinia` in global context (here is `router.ts`), we must call pinia stores inside a method then return it (here is `router`).

But I got a wierd bug when using like following:

```ts
export const router = () => {
  const routerInner = createRouter({
    history: createWebHistory(import.meta.env.BASE_URL),
    routes: routes
  })

  const userStore = useUserStore()

  routerInner.beforeEach((to, from, next) => {
    switch (to.name) {
      case 'Login':
        next()
        break
      default:
        if (userStore.token !== '') {
          next()
        } else {
          next('/login')
        }
        break
    }
  })
  return routerInner
}
```

!!! Error You must declare the `routerInner` outside the `router` function.