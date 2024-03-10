#front-end/css #front-end/vue
## 2023.11.17

## Change scroll bar styles

```scss
::-webkit-scrollbar {
  display: none; // don't show scrollbar
  width: 12px;
  background-color: var(--vt-c-divider-light-2);
}

::-webkit-scrollbar-thumb {
  border-radius: 50px;
  background-color: var(--vt-c-divider-dark-1);
  cursor: pointer;
}
```

## Remove global scrollbar 

```scss
html {
    overflow: hidden;
}
```