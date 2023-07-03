//
// Copyright (c) 2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

//------------------------------------------------------------------------------
//
// Example: HTTP server, fast
//
//------------------------------------------------------------------------------

#include <mysql.h>
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <thread>
#include <atomic>
#include <stdio.h>
#include <ctime>

#include "../include/fields_alloc.hpp"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <list>
#include <memory>
#include <string>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

// Return a reasonable mime type based on the extension of a file.
beast::string_view
mime_type(beast::string_view path)
{
    using beast::iequals;
    auto const ext = [&path]
    {
        auto const pos = path.rfind(".");
        if(pos == beast::string_view::npos)
            return beast::string_view{};
        return path.substr(pos);
    }();
    if(iequals(ext, ".htm"))  return "text/html";
    if(iequals(ext, ".html")) return "text/html";
    if(iequals(ext, ".php"))  return "text/html";
    if(iequals(ext, ".css"))  return "text/css";
    if(iequals(ext, ".txt"))  return "text/plain";
    if(iequals(ext, ".js"))   return "application/javascript";
    if(iequals(ext, ".json")) return "application/json";
    if(iequals(ext, ".xml"))  return "application/xml";
    if(iequals(ext, ".swf"))  return "application/x-shockwave-flash";
    if(iequals(ext, ".flv"))  return "video/x-flv";
    if(iequals(ext, ".png"))  return "image/png";
    if(iequals(ext, ".jpe"))  return "image/jpeg";
    if(iequals(ext, ".jpeg")) return "image/jpeg";
    if(iequals(ext, ".jpg"))  return "image/jpeg";
    if(iequals(ext, ".gif"))  return "image/gif";
    if(iequals(ext, ".bmp"))  return "image/bmp";
    if(iequals(ext, ".ico"))  return "image/vnd.microsoft.icon";
    if(iequals(ext, ".tiff")) return "image/tiff";
    if(iequals(ext, ".tif"))  return "image/tiff";
    if(iequals(ext, ".svg"))  return "image/svg+xml";
    if(iequals(ext, ".svgz")) return "image/svg+xml";
    return "application/text";
}

class http_worker
{
public:
    http_worker(http_worker const&) = delete;
    http_worker& operator=(http_worker const&) = delete;

    http_worker(tcp::acceptor& acceptor, const std::string& doc_root) :
        acceptor_(acceptor),
        doc_root_(doc_root)
    {
    }

    void start()
    {
        accept();
        check_deadline();
    }

private:
    using alloc_t = fields_alloc<char>;
    //using request_body_t = http::basic_dynamic_body<beast::flat_static_buffer<1024 * 1024>>;
    using request_body_t = http::string_body;

    // The acceptor used to listen for incoming connections.
    tcp::acceptor& acceptor_;

    // The path to the root of the document directory.
    std::string doc_root_;

    // The socket for the currently connected client.
    tcp::socket socket_{acceptor_.get_executor()};

    // The buffer for performing reads
    beast::flat_static_buffer<8192> buffer_;

    // The allocator used for the fields in the request and reply.
    alloc_t alloc_{8192};

    // The parser for reading the requests
    boost::optional<http::request_parser<request_body_t, alloc_t>> parser_;

    // The timer putting a time limit on requests.
    net::basic_waitable_timer<std::chrono::steady_clock> request_deadline_{
        acceptor_.get_executor(), (std::chrono::steady_clock::time_point::max)()};

    // The string-based response message.
    boost::optional<http::response<http::string_body, http::basic_fields<alloc_t>>> string_response_;

    // The string-based response serializer.
    boost::optional<http::response_serializer<http::string_body, http::basic_fields<alloc_t>>> string_serializer_;

    // The file-based response message.
    boost::optional<http::response<http::file_body, http::basic_fields<alloc_t>>> file_response_;

    // The file-based response serializer.
    boost::optional<http::response_serializer<http::file_body, http::basic_fields<alloc_t>>> file_serializer_;

    void accept()
    {
        // Clean up any previous connection.
        beast::error_code ec;
        socket_.close(ec);
        buffer_.consume(buffer_.size());

        acceptor_.async_accept(
            socket_,
            [this](beast::error_code ec)
            {
                if (ec)
                {
                    accept();
                }
                else
                {
                    // Request must be fully processed within 60 seconds.
                    request_deadline_.expires_after(
                        std::chrono::seconds(60));

                    read_request();
                }
            });
    }

    void read_request()
    {
        // On each read the parser needs to be destroyed and
        // recreated. We store it in a boost::optional to
        // achieve that.
        //
        // Arguments passed to the parser constructor are
        // forwarded to the message object. A single argument
        // is forwarded to the body constructor.
        //
        // We construct the dynamic body with a 1MB limit
        // to prevent vulnerability to buffer attacks.
        //
        parser_.emplace(
            std::piecewise_construct,
            std::make_tuple(),
            std::make_tuple(alloc_));

        http::async_read(
            socket_,
            buffer_,
            *parser_,
            [this](beast::error_code ec, std::size_t)
            {
                if (ec)
                    accept();
                else
                    process_request(parser_->get());
            });
    }

    void process_request(http::request<request_body_t, http::basic_fields<alloc_t>> const& req)
    {
        switch (req.method())
        {
            case http::verb::get:{
                std::string reg_target(req.target());
                if (reg_target == "/take_picure")
                {
            	    cv::VideoCapture camera(0);
		    cv::Mat frame;
		    camera >> frame;
		    std::time_t current_time = std::time(nullptr);
		    std::string time_str = std::asctime(std::localtime(&current_time));

		    int baseline = 0;
		    cv::Size textSize = cv::getTextSize(time_str, cv::FONT_HERSHEY_SIMPLEX, 1, 2, &baseline);
		    cv::Point textOrg((frame.cols - textSize.width) / 2, textSize.height + 10);
		    cv::putText(frame, time_str, textOrg, cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2, 8);
		    cv::imwrite("image.png", frame);
		    send_file("/image.png");
                }else if (reg_target.rfind("/show_face_table", 0) != std::string::npos){
            	    int index = reg_target.find('?');
		    std::string number_string = reg_target.substr(index + 1);
		    int q = number_string.length();
		    char num_char[q + 1];
		    strcpy(num_char, number_string.c_str());
		    int n = std::stoi(num_char);
		    std::string data = read_data(n);
		    send_text_response(data);
                }
            
                break;
            }

            default:
                // We return responses indicating an error if
                // we do not recognize the request method.
                send_bad_response(
                    http::status::bad_request,
                    "Invalid request-method '" + std::string(req.method_string()) + "'\r\n");
                break;
        }
    }
    
    std::string read_data(int n)
    {

	MYSQL* dB;
	dB = mysql_init(NULL);
        if (dB == NULL)
        {
            fprintf(stderr, "%s\n", mysql_error(dB));
            exit(1);
        }
        if (mysql_real_connect(dB, "localhost", "root", "mohsh2002",
                NULL, 0, NULL, 0) == NULL)
        {
            fprintf(stderr, "%s\n", mysql_error(dB));
            exit(1);
        }
        if (mysql_query(dB, "USE PROJECT"))
        {
            fprintf(stderr, "%s\n", mysql_error(dB));
            exit(1);
        }
        MYSQL_ROW row;
        MYSQL_FIELD *field;
        //check whether its empty or not
        int c = -1;
	// Declare a string to store the query
	std::string query = "SELECT COUNT(*) FROM FACES";
	int q = query.length();
        char query_char[q + 1];
        strcpy(query_char, query.c_str());
        if (mysql_query(dB, query_char))
        {
            fprintf(stderr, "%s\n", mysql_error(dB));
            exit(1);
        }
        MYSQL_RES *result = mysql_store_result(dB);
        if (result == NULL)
        {
            fprintf(stderr, "%s\n", mysql_error(dB));
            exit(1);
        }
        while ((row = mysql_fetch_row(result)))
        {
            while(field = mysql_fetch_field(result));
            std::string no(row[0]);
            c = std::stoi(no);            
        }
	mysql_free_result(result);
        std::string data_string("Your selected data:\n");
        if (c > 0 && c >= n && n > 0)
        {
            //get data
            std::string command("SELECT * FROM FACES ORDER BY id DESC LIMIT ");
            command = command + std::to_string(n);
            q = command.length();
            char command_char[q + 1];
            strcpy(command_char, command.c_str());
            if (mysql_query(dB, command_char))
            {
		fprintf(stderr, "%s\n", mysql_error(dB));
                exit(1);
            }
            result = mysql_store_result(dB);
            if (result == NULL)
            {
                fprintf(stderr, "%s\n", mysql_error(dB));
                exit(1);
            }
            int num_fields = 3;
            while ((row = mysql_fetch_row(result)))
            {
                for(int i = 0; i < num_fields; i++)
                {
                    if (i == 0)
                    {
                        while(field = mysql_fetch_field(result))
                        {
                            std::string name(field->name);
                            data_string = data_string + "\t" + name;
                        }
                        data_string = data_string + "\n";
                    }
                    std::string str(row[i]);
                    if (i == 2)
                        data_string = data_string + "\t\t" + str;    
                    else
                        data_string = data_string + "\t" + str;
                }
            }
        }else{
            printf("Empty DataBase!");
            return ":(";
        }
        mysql_close(dB);
        return data_string;
    }
    
    void send_text_response(std::string const& text)
    {
        string_response_.emplace(
            std::piecewise_construct,
            std::make_tuple(),
            std::make_tuple(alloc_));    
            string_response_->result(http::status::ok);
            string_response_->keep_alive(false);
            string_response_->set(http::field::server, "Beast");
            string_response_->set(http::field::content_type, "text/plain");
            string_response_->body() = text;
            string_response_->prepare_payload(); 
            string_serializer_.emplace(*string_response_);   
            http::async_write(
                socket_,
                *string_serializer_,
                [this](beast::error_code ec, std::size_t)
                {
                    socket_.shutdown(tcp::socket::shutdown_send, ec);
                    string_serializer_.reset();
                    string_response_.reset();
                    accept();
                });
    }

    void send_bad_response(
        http::status status,
        std::string const& error)
    {
        string_response_.emplace(
            std::piecewise_construct,
            std::make_tuple(),
            std::make_tuple(alloc_));

        string_response_->result(status);
        string_response_->keep_alive(false);
        string_response_->set(http::field::server, "Beast");
        string_response_->set(http::field::content_type, "text/plain");
        string_response_->body() = error;
        string_response_->prepare_payload();

        string_serializer_.emplace(*string_response_);

        http::async_write(
            socket_,
            *string_serializer_,
            [this](beast::error_code ec, std::size_t)
            {
                socket_.shutdown(tcp::socket::shutdown_send, ec);
                string_serializer_.reset();
                string_response_.reset();
                accept();
            });
    }

    void send_file(beast::string_view target)
    {
        // Request path must be absolute and not contain "..".
        if (target.empty() || target[0] != '/' || target.find("..") != std::string::npos)
        {
            send_bad_response(
                http::status::not_found,
                "File not found\r\n");
            return;
        }

        std::string full_path = doc_root_;
        full_path.append(
            target.data(),
            target.size());

        http::file_body::value_type file;
        beast::error_code ec;
        file.open(
            full_path.c_str(),
            beast::file_mode::read,
            ec);
        if(ec)
        {
            send_bad_response(
                http::status::not_found,
                "File not found\r\n");
            return;
        }

        file_response_.emplace(
            std::piecewise_construct,
            std::make_tuple(),
            std::make_tuple(alloc_));

        file_response_->result(http::status::ok);
        file_response_->keep_alive(false);
        file_response_->set(http::field::server, "Beast");
        file_response_->set(http::field::content_type, mime_type(std::string(target)));
        file_response_->body() = std::move(file);
        file_response_->prepare_payload();

        file_serializer_.emplace(*file_response_);

        http::async_write(
            socket_,
            *file_serializer_,
            [this](beast::error_code ec, std::size_t)
            {
                socket_.shutdown(tcp::socket::shutdown_send, ec);
                file_serializer_.reset();
                file_response_.reset();
                accept();
            });
    }

    void check_deadline()
    {
        // The deadline may have moved, so check it has really passed.
        if (request_deadline_.expiry() <= std::chrono::steady_clock::now())
        {
            // Close socket to cancel any outstanding operation.
            beast::error_code ec;
            socket_.close();

            // Sleep indefinitely until we're given a new deadline.
            request_deadline_.expires_at(
                std::chrono::steady_clock::time_point::max());
        }

        request_deadline_.async_wait(
            [this](beast::error_code)
            {
                check_deadline();
            });
    }
};

int main(int argc, char* argv[])
{
    try
    {
        // Check command line arguments.
        if (argc != 6)
        {
            std::cerr << "Usage: http_server_fast <address> <port> <doc_root> <num_workers> {spin|block}\n";
            std::cerr << "  For IPv4, try:\n";
            std::cerr << "    http_server_fast 0.0.0.0 80 . 100 block\n";
            std::cerr << "  For IPv6, try:\n";
            std::cerr << "    http_server_fast 0::0 80 . 100 block\n";
            return EXIT_FAILURE;
        }

        auto const address = net::ip::make_address(argv[1]);
        unsigned short port = static_cast<unsigned short>(std::atoi(argv[2]));
        std::string doc_root = argv[3];
        int num_workers = std::atoi(argv[4]);
        bool spin = (std::strcmp(argv[5], "spin") == 0);

        net::io_context ioc{1};
        tcp::acceptor acceptor{ioc, {address, port}};

        std::list<http_worker> workers;
        for (int i = 0; i < num_workers; ++i)
        {
            workers.emplace_back(acceptor, doc_root);
            workers.back().start();
        }

        if (spin)
          for (;;) ioc.poll();
        else
          ioc.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
