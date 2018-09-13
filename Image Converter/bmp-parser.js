const fs = require("fs")
const bmp = require("bmp-js")

let dev = 0

let path = "bmp"


fs.readdir(path, (err, files) => {
	files.forEach((file, fi, arr) => {

		const fn = file.substr(0, file.length - 4)
		console.log("FILE: " + fn)

		let bmpBuffer = fs.readFileSync(path + '/' + fn + '.bmp')
		let bmpData = bmp.decode(bmpBuffer)

		//bmpData={data:Buffer,width:Number,height:Height} 

		let pre_str = "const uint8_t bmp_" + fn + "[] PROGMEM = { \n\t" //0xAA, 0xA8, //
		let post_str = " \n}; \n\nconst Bitmap Bitmap_" + fn + " PROGMEM = { \n\t(uint8_t *)bmp_" + fn + ",\t//buffer\n\t" + bmpData.width + ",\t//width\n\t" + bmpData.height + "\t//height\n};";

		let mid_str = ""

		for (let i = 0; i < bmpData.data.length; i += 8 * 4) {
			let byte = 0x00
			for (let px = 0; px < 8 * 4; px += 4) {
				let r = 255 - bmpData.data.readUInt8(i + px)
				let g = 255 - bmpData.data.readUInt8(i + px + 1)
				let b = 255 - bmpData.data.readUInt8(i + px + 2)
				//let a = bmpData.data.readUInt8(i + px + 3)

				if (dev) console.log(i + px, r,g,b)

				if (r + g + b >= 255 * 3 / 2) {
					byte |= (1 << (7 - px / 4))
					if (dev) console.log(byte.toString(2))
					//if (dev) console.log(1)
				} else {
					//if (dev) console.log(0)
				}
			}

			if (dev) console.log(byte.toString(2))
			if (dev) console.log("############")

			mid_str += "0x" + byte.toString(16)
			mid_str += ", "
		}
		mid_str = mid_str.substr(0, mid_str.length - 2)

		if (dev) console.log(mid_str)

		fs.writeFileSync('../Inc/Display/bitmaps/' + fn + ".h", pre_str + mid_str + post_str, {encoding: "utf8"})


		console.log("DONE")
	})
})

