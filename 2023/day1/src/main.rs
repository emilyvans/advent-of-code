use std::fs::read_to_string;

fn main() {
    let file_path = "./input.txt";
    let content = read_to_string(file_path).unwrap();
    let mut numbers: Vec<u32> = Vec::new();

    for line in content.lines() {
        let mut first: Option<u32> = None;
        let mut last: Option<u32> = None;

        let new_line = line
            .replace("one", "o1e")
            .replace("two", "t2o")
            .replace("three", "t3e")
            .replace("four", "f4r")
            .replace("five", "f5e")
            .replace("six", "s6x")
            .replace("seven", "s7n")
            .replace("eight", "e8t")
            .replace("nine", "n9e");

        println!("current line: {}", new_line);

        for char in new_line.chars() {
            match char {
                '0'..='9' => {
                    if first == None {
                        let value = char.to_digit(10);
                        first = value;
                        last = value;
                    } else {
                        last = char.to_digit(10);
                    }
                }
                _ => {}
            }
        }
        if let Some(tens) = first {
            if let Some(ones) = last {
                println!(
                    "first: {}, last: {}, combined: {}",
                    tens,
                    ones,
                    tens * 10 + ones
                );
                numbers.push(tens * 10 + ones);
            }
        }
    }

    let mut final_number = 0;

    for number in numbers {
        final_number += number;
    }

    println!("the file path is {}", file_path);
    println!("the result is {}", final_number);
}
