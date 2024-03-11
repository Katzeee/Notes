#typescript

## `tpye` and `interface`

`interface` works like an abstruct class while `type` just indicates a new type.

## `typeof`

```ts
declare function createFetch(config?: CreateFetchOptions): typeof useFetch;
declare function useFetch<T>(url: MaybeRefOrGetter<string>): UseFetchReturn<T> & PromiseLike<UseFetchReturn<T>>;
declare function useFetch<T>(url: MaybeRefOrGetter<string>, useFetchOptions: UseFetchOptions): UseFetchReturn<T> & PromiseLike<UseFetchReturn<T>>;
declare function useFetch<T>(url: MaybeRefOrGetter<string>, options: RequestInit, useFetchOptions?: UseFetchOptions): UseFetchReturn<T> & PromiseLike<UseFetchReturn<T>>;
```
The first function returns a value of the type of `useFetch` where `useFetch` is a function which has three overload declarations.

## `UseFetchReturn` in `vueuse`

```ts
interface UseFetchReturn<T> {
    /**
     * Indicates if the fetch request has finished
     */
    isFinished: Ref<boolean>;
    /**
     * The statusCode of the HTTP fetch response
     */
    statusCode: Ref<number | null>;
    /**
     * The raw response of the fetch response
     */
    response: Ref<Response | null>;
    /**
     * Any fetch errors that may have occurred
     */
    error: Ref<any>;
    /**
     * The fetch response body on success, may either be JSON or text
     */
    data: Ref<T | null>;
    /**
     * Indicates if the request is currently being fetched.
     */
    isFetching: Ref<boolean>;
    /**
     * Indicates if the fetch request is able to be aborted
     */
    canAbort: ComputedRef<boolean>;
    /**
     * Indicates if the fetch request was aborted
     */
    aborted: Ref<boolean>;
    /**
     * Abort the fetch request
     */
    abort: Fn;
    /**
     * Manually call the fetch
     * (default not throwing error)
     */
    execute: (throwOnFailed?: boolean) => Promise<any>;
    /**
     * Fires after the fetch request has finished
     */
    onFetchResponse: EventHookOn<Response>;
    /**
     * Fires after a fetch request error
     */
    onFetchError: EventHookOn;
    /**
     * Fires after a fetch has completed
     */
    onFetchFinally: EventHookOn;
    get(): UseFetchReturn<T> & PromiseLike<UseFetchReturn<T>>;
    post(payload?: MaybeRefOrGetter<unknown>, type?: string): UseFetchReturn<T> & PromiseLike<UseFetchReturn<T>>;
    put(payload?: MaybeRefOrGetter<unknown>, type?: string): UseFetchReturn<T> & PromiseLike<UseFetchReturn<T>>;
    delete(payload?: MaybeRefOrGetter<unknown>, type?: string): UseFetchReturn<T> & PromiseLike<UseFetchReturn<T>>;
    patch(payload?: MaybeRefOrGetter<unknown>, type?: string): UseFetchReturn<T> & PromiseLike<UseFetchReturn<T>>;
    head(payload?: MaybeRefOrGetter<unknown>, type?: string): UseFetchReturn<T> & PromiseLike<UseFetchReturn<T>>;
    options(payload?: MaybeRefOrGetter<unknown>, type?: string): UseFetchReturn<T> & PromiseLike<UseFetchReturn<T>>;
    json<JSON = any>(): UseFetchReturn<JSON> & PromiseLike<UseFetchReturn<JSON>>;
    text(): UseFetchReturn<string> & PromiseLike<UseFetchReturn<string>>;
    blob(): UseFetchReturn<Blob> & PromiseLike<UseFetchReturn<Blob>>;
    arrayBuffer(): UseFetchReturn<ArrayBuffer> & PromiseLike<UseFetchReturn<ArrayBuffer>>;
    formData(): UseFetchReturn<FormData> & PromiseLike<UseFetchReturn<FormData>>;
}
```

`UseFetchReturn.get` returns a `UseFetchReturn` because you may use get request to get a url then send a post request or something.