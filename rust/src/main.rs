use std::env;
use std::process;
use b12od::OfferDecoder;

fn main() {
    let args: Vec<String> = env::args().collect();

    if args.len() != 2 {
        eprintln!("Usage: {} [offer_string]", args[0]);
        process::exit(1);
    }

    let offer_string = &args[1];
    let offer_decoder = OfferDecoder::new();
    println!("{}", offer_decoder.decode(offer_string));
}
