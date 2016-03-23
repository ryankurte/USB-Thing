# USBThing Bootloader

Based on Silabs AN0042 from: https://www.silabs.com/products/mcu/Pages/32-bit-mcu-application-notes.aspx  
https://www.silabs.com/Support%20Documents/TechnicalDocs/AN0042.pdf  

See resources/ for more information

## Usage
1. Open port with `minicom --device /dev/tty.whatever`
2. Initialize transfer with `u`
3. Use Meta+Z to open minicom menu
4. Use 'S' to upload a file
5. Select file and upload
6. Press 'b' to boot