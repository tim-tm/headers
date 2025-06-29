class Hashtable {
    constructor(instance) {
        this.memory = new DataView(instance.exports.memory.buffer)
        this.ht_insert = instance.exports.ht_insert
        this.ht_get_value = instance.exports.ht_get_value
        this.alloc_loc = 4
        this.table = instance.exports.get_table()
        this.codes = [
            "SUCCESS",
            "ERROR_INVALID_INPUT",
            "ERROR_HASH_FUNC",
            "ERROR_TABLE_FULL"
        ]
        console.log("--- Hashtable initialized ---")
        console.log(">>> memory.byteLength = ", this.memory.byteLength)
        console.log(">>> memory.byteOffset = ", this.memory.byteOffset)
        console.log(">>> ht_insert         = ", this.ht_insert)
        console.log(">>> ht_get_value      = ", this.ht_get_value)
        console.log(">>> alloc_loc         = ", this.alloc_loc)
        console.log(">>> table             = ", this.table)
        console.log(">>> codes             = ", this.codes)
    }

    _get_key_ptr(key) {
        console.log("Allocating new key: ", key)
        console.log("alloc_loc before: ", this.alloc_loc)
        const keyPtr = this.alloc_loc
        for (let i = 0; i < key.length; i++) {
            this.memory.setInt8(keyPtr + i, key.charCodeAt(i), true)
        }
        this.memory.setInt8(keyPtr + key.length, 0, true)
        this.alloc_loc += key.length + 1
        console.log("alloc_loc after: ", this.alloc_loc)
        console.log("keyPtr: ", keyPtr)
        return keyPtr
    }

    _get_value_ptr(value) {
        console.log("Allocating new value: ", value)
        console.log("alloc_loc before: ", this.alloc_loc)
        const valuePtr = this.alloc_loc
        this.memory.setInt32(valuePtr, value, true)
        this.alloc_loc += 4
        console.log("alloc_loc after: ", this.alloc_loc)
        console.log("valuePtr: ", valuePtr)
        return valuePtr
    }

    get_code_name(code) {
        return this.codes[code]
    }

    insert(key, value) {
        console.log("--- Performing insertion ---")
        const keyPtr = this._get_key_ptr(key)
        const intPtr = this._get_value_ptr(value)
        return this.ht_insert(this.table, keyPtr, intPtr)
    }

    get_value(key) {
        console.log("--- Querying value ---")
        const strPtr = this._get_key_ptr(key)
        const resultPtr = this.ht_get_value(this.table, strPtr)
        return this.memory.getInt32(resultPtr, true)
    }
}

(async () => {
    const response = await fetch("build/ht.wasm")
    const buffer = await response.arrayBuffer()
    const { instance } = await WebAssembly.instantiate(buffer)

    const hashtable = new Hashtable(instance)
    console.log("Result of `insert('Hello', 1337)`: ", hashtable.get_code_name(hashtable.insert("Hello", 1337)))
    console.log("Result of `insert('World', 69420)`: ", hashtable.get_code_name(hashtable.insert("World", 69420)))
    console.log("Result of `get_value('Hello')`: ", hashtable.get_value("Hello"))
    console.log("Result of `get_value('World')`: ", hashtable.get_value("World"))
})();
