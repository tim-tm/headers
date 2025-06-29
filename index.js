const html_dump = document.getElementById("dump")
const html_insert_key = document.getElementById("ht_insert_key")
const html_insert_value = document.getElementById("ht_insert_value")
const html_get_key = document.getElementById("ht_get_key")
const html_button_insert = document.getElementById("ht_button_insert")
const html_button_get = document.getElementById("ht_button_get")
const html_ht = document.getElementById("ht")

class Hashtable {
    constructor(instance, memory_start = 2) {
        this.memory = new DataView(instance.exports.memory.buffer)
        this.ht_insert = instance.exports.ht_insert
        this.ht_get_value = instance.exports.ht_get_value

        // defaults to 2, should not default to 0
        // since 0 = NULL in C, which would result
        // in ERROR_INVALID_INPUT
        this.memory_start = memory_start
        this.alloc_loc = this.memory_start
        this.table_ptr = instance.exports.get_table()

        // We use int* as data, to get the data_len
        // attribute of the ht_state structure, we
        // can simply add 4 (sizeof(int*)) to the
        // table pointer
        this.table_data_len = this.memory.getUint32(this.table_ptr+4, true)
        this.codes = [
            "SUCCESS",
            "ERROR_INVALID_INPUT",
            "ERROR_HASH_FUNC",
            "ERROR_TABLE_FULL"
        ]

        console.log("--- Hashtable initialized ---")
        const msgs = [
            `memory.byteLength = ${this.memory.byteLength}`,
            `memory.byteOffset = ${this.memory.byteOffset}`,
            `alloc_loc         = ${this.alloc_loc}`,
            `table_ptr         = ${this.table_ptr}`,
            `table_data_len    = ${this.table_data_len}`,
            `codes             = ${this.codes}`
        ]
        for (let i = 0; i < msgs.length; i++) {
            console.log(">>> ", msgs[i])
            let li = document.createElement("li")
            li.innerText = msgs[i]
            html_ht.appendChild(li)
        }
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

    get_memory_start() {
        return this.memory_start
    }

    get_memory_end() {
        return this.alloc_loc
    }

    get_memory() {
        return this.memory
    }

    get_code_name(code) {
        return this.codes[code]
    }

    insert(key, value) {
        console.log("--- Performing insertion ---")
        const keyPtr = this._get_key_ptr(key)
        const intPtr = this._get_value_ptr(value)
        return this.ht_insert(this.table_ptr, keyPtr, intPtr)
    }

    get_value(key) {
        console.log("--- Querying value ---")
        const strPtr = this._get_key_ptr(key)
        const resultPtr = this.ht_get_value(this.table_ptr, strPtr)
        return this.memory.getInt32(resultPtr, true)
    }
}

function hexDump(view, offset = 0, length = 64) {
    // remove childs
    html_dump.innerHTML = "";
    for (let i = offset; i < offset + length; i += 8) {
        let hexBytes = "";
        let asciiStr = "";
        for (let j = 0; j < 8 && i + j < view.byteLength; j++) {
            const byte = view.getUint8(i + j)
            hexBytes += byte.toString(16).padStart(2, '0')
            hexBytes += " "
            asciiStr += (byte >= 32 && byte <= 126) ? String.fromCharCode(byte) : "."
        }
        const str = `0x${i.toString(16).padStart(4, '0')}: ${hexBytes} ${asciiStr}`
        let li = document.createElement("li")
        li.innerText = str
        html_dump.appendChild(li)
    }
}

let wasm_instance = undefined;
let hashtable = undefined;

async function get_wasm_instance() {
    const response = await fetch("build/ht.wasm")
    const buffer = await response.arrayBuffer()
    const { instance } = await WebAssembly.instantiate(buffer)
    wasm_instance = instance
}

get_wasm_instance().catch(err => {
    console.log("Failed to load WASM:", err)
})

html_button_insert.onclick = () => {
    if (hashtable === undefined) hashtable = new Hashtable(wasm_instance)

    window.alert(hashtable.get_code_name(hashtable.insert(html_insert_key.value, html_insert_value.value)))
    hexDump(hashtable.get_memory())
}

html_button_get.addEventListener("click", () => {
    if (hashtable === undefined) hashtable = new Hashtable(wasm_instance)

    window.alert(hashtable.get_value(html_get_key.value))
    hexDump(hashtable.get_memory(), hashtable.get_memory_start(), hashtable.get_memory_end())
})
