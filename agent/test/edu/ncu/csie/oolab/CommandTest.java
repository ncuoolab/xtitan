/*
Copyright (c) 2011 Wei Cheng Pan <legnaleurc@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
package edu.ncu.csie.oolab;

import static org.junit.Assert.*;

import java.nio.charset.Charset;
import java.util.HashMap;

import org.apache.commons.codec.binary.Base64;
import org.junit.Test;

import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;

public class CommandTest {

	private CommandParser parser_;
	private Gson json_;

	public CommandTest() {
		this.parser_ = new CommandParser();
		this.json_ = new Gson();
	}

	@Test
	public void testExecute() {
		HashMap< String, Object > args = new HashMap< String, Object >();
		args.put( "command", "execute" );
		args.put( "script", "print \'Hello, world!\'\n" );
		String b64Data = this.json_.toJson( args );
		b64Data = new String( Base64.encodeBase64( b64Data.getBytes( Charset.forName( "UTF-8" ) ) ), Charset.forName( "UTF-8" ) );
		b64Data = this.parser_.execute( b64Data );
		b64Data = new String( Base64.decodeBase64( b64Data.getBytes( Charset.forName( "UTF-8" ) ) ), Charset.forName( "UTF-8" ) );
		args = this.json_.fromJson( b64Data, ( new TypeToken< HashMap< String, Object > >() {} ).getType() );
		assertTrue( ( Boolean ) args.get( "success" ) );
	}

	@Test
	public void testExit() {
		HashMap< String, Object > args = new HashMap< String, Object >();
		args.put( "command", "exit" );
		String b64Data = this.json_.toJson( args );
		b64Data = new String( Base64.encodeBase64( b64Data.getBytes( Charset.forName( "UTF-8" ) ) ), Charset.forName( "UTF-8" ) );
		b64Data = this.parser_.execute( b64Data );
		b64Data = new String( Base64.decodeBase64( b64Data.getBytes( Charset.forName( "UTF-8" ) ) ), Charset.forName( "UTF-8" ) );
		args = this.json_.fromJson( b64Data, ( new TypeToken< HashMap< String, Object > >() {} ).getType() );
		assertEquals( args, null );
	}

}
