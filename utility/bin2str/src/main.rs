fn byte2char(x: &[u8]) -> String {
    let mut s = String::with_capacity(x.len() * 2);
    s.push_str("    ");

    let mut counter = s.len();

    for b in x {
        let push = format!("{}, ", *b as i32);
        s.extend(push.chars());

        counter += push.len();

        if counter > 80 {
            s.pop();
            let pos = s.len() - 1 - s.chars().rev().position(|c| c == ' ' ).unwrap();
            s.insert_str(pos, "\n   ");
            s.push(' ');
            counter = s.len() - pos - 1;
        }
    }

    s
}

fn convert_file(x: &mut dyn std::io::Read) -> std::io::Result<String> {
    let mut bytes = Vec::new();
    x.read_to_end(&mut bytes)?;

    let mut bytestr = byte2char(bytes.as_slice());
    bytestr.pop(); bytestr.pop();

    Ok(bytestr)
}

fn main() -> std::io::Result<()> {
    // use std::env;
    use std::path::Path;
    use std::fs::File;
    use std::io::Write;

    // let mut args: Vec<String> = env::args().collect();

    // if args.len() < 2 {
    //     eprintln!("Insufficient arguments provided!");
    //     eprintln!("Usage:");
    //     eprintln!("\t{} file1 [file2 ...]", args[0]);
    // }

    // args.remove(0);

    // for a in args {
    let a = String::from("input.wav");
        let mut inp_file: File = File::open(Path::new(a.as_str()))?;

        let mut binstr = convert_file(&mut inp_file)?;
        binstr.insert_str(0, "pub static wav: [u8;_] = [\n");
        binstr.push_str("\n];\n");

        let mut out_file = std::fs::File::create("output.rs")?;
        out_file.write(binstr.as_bytes())?;
    // }

    Ok(())
}
