package com.chenshuo.muduo.example.multiplexer.testcase;

import java.nio.charset.Charset;
import java.util.regex.Matcher;

import org.jboss.netty.buffer.ChannelBuffer;

import com.chenshuo.muduo.example.multiplexer.DataEvent;
import com.chenshuo.muduo.example.multiplexer.Event;
import com.chenshuo.muduo.example.multiplexer.EventSource;
import com.chenshuo.muduo.example.multiplexer.MockClient;
import com.chenshuo.muduo.example.multiplexer.TestCase;

public class TestOneClientBothSend extends TestCase {

    @Override
    public void run() {
        if (!queue.isEmpty())
            fail("EventQueue is not empty");

        // step 1
        MockClient client = god.newClient();
        Event ev = queue.take();
        DataEvent de = (DataEvent) ev;
        assertEquals(EventSource.kBackend, de.source);

        Matcher m = god.commandChannel.matcher(de.getString());
        if (!m.matches())
            fail("command channel message doesn't match.");

        final int connId = Integer.parseInt(m.group(1));
        assertTrue(connId > 0);
        client.setId(connId);

        assertEquals("UP", m.group(2));

        // step 2
        ChannelBuffer buf = client.send("hello");
        de = (DataEvent) queue.take();
        assertEquals(EventSource.kBackend, de.source);
        assertEquals(connId, de.whichClient);
        assertEquals(buf, de.data);
        System.out.println(de.data.toString(Charset.defaultCharset()));

        // step 3
        buf = backend.sendToClient(connId, "World!");
        de = (DataEvent) queue.take();
        assertEquals(EventSource.kClient, de.source);
        assertEquals(connId, de.whichClient);
        assertEquals(buf, de.data);
        System.out.println(de.data.toString(Charset.defaultCharset()));

        // step 4
        client.disconnect();
        de = (DataEvent) queue.take();
        assertEquals(EventSource.kBackend, de.source);
        m = god.commandChannel.matcher(de.getString());
        if (!m.matches())
            fail("command channel message doesn't match.");

        assertEquals(connId, Integer.parseInt(m.group(1)));
        assertEquals("DOWN", m.group(2));
    }
}
