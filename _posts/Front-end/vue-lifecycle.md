#front-end/vue
## 2023.11.22

## How to get the width of a dom element

```html
<template>
    <div class="right" ref="rightRef"></div>
</template>

<script setup lang="ts">
const rightRef = ref<HTMLElement | null>(null)
const rightWidth = ref(0)

onMounted(() => {
  rightWidth.value = rightRef.value?.offsetWidth as number
})
</script>
```

## How to define a watched varible

```html
<script setup lang="ts">
watch(
  () => diary.value.tags as number[],
  (newValue, oldValue) => {
    if (newValue.length === 0) {
      ;(diary.value.tags as number[]).push(0)
      return
    }
    if (newValue.length > 1) {
      const index = (diary.value.tags as number[]).indexOf(0)
      if (index !== -1) {
        ;(diary.value.tags as number[]).splice(index, 1)
      }
    }
  }
)
</script>
```

!!! Assign to the ref varible rather than `newValue`